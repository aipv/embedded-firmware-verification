#include "esp_log.h"
#include "esp_err.h"
#include "esp_image_format.h"
#include "esp_flash_partitions.h"
#include "esp_rom_spiflash.h"
#include "esp_rom_sys.h"
#include "bootloader_utility.h"
#include "bootloader_sha.h"
#include "efv_sha256.h"
#include "efv_ed25519.h"
#include "efv_manifest.h"

static const char *TAG = "EFV_BOOT_VERIFY";

static const uint8_t pub_key[32] = {
    0xb4, 0x16, 0xbb, 0x71, 0x3d, 0x1a, 0x32, 0x99, 0x0f, 0x89, 0x33, 0x8c, 0x0c, 0x6d, 0xa2, 0xae,
    0x3d, 0x48, 0xdf, 0xef, 0x5d, 0x8e, 0xad, 0xf0, 0x25, 0xbf, 0xd0, 0x91, 0x91, 0xb8, 0x80, 0xa2
};

#define ERROR_CODE_CHECK(cond, msg)           \
    do {                                           \
        if (cond) {                                \
            ESP_LOGE(TAG, "%s", msg);              \
            return -1;                            \
        }                                          \
    } while (0)

void bootloader_hooks_include(void)
{
}

int bootloader_sha256_hash_stream(uint8_t *hash, uint32_t fw_offset, uint32_t fw_len)
{
    uint8_t buf[2048] __attribute__((aligned(4)));
    uint32_t readSize = 0;

    bootloader_sha256_handle_t handle;
    handle = bootloader_sha256_start();

    while (readSize < fw_len)
    {
        uint32_t remaining = fw_len - readSize;
        uint32_t bytes = remaining > sizeof(buf) ? sizeof(buf) : remaining;

        int ret = esp_rom_spiflash_read(fw_offset + readSize, (uint32_t *)buf, bytes);
        ERROR_CODE_CHECK(ret != 0, "flash read failed");

        bootloader_sha256_data(handle, buf, bytes);

        readSize += bytes;
    }

    bootloader_sha256_finish(handle, hash);

    return 0;
}

int bootloader_partition_verify(const bootloader_state_t *bs, int index)
{
    const esp_partition_pos_t *part = &bs->ota[index];
    const char *name = (index == 0) ? "ota_0" : "ota_1";
    uint32_t fw_offset = part->offset;
    ESP_LOGI(TAG, "========== Verify %s: index=%d offset=0x%08lx size=0x%08lx ==========",name, index, fw_offset, part->size);
    ERROR_CODE_CHECK(part->size <= EFV_IMAGE_METADATA_SIZE, "Invalid Partition Size");
    ESP_LOGI(TAG, "[1/4] Partition OK");

    uint8_t metadata[EFV_IMAGE_METADATA_SIZE] __attribute__((aligned(4)));
    efv_manifest_t *header = (efv_manifest_t *)metadata;
    int ret = esp_rom_spiflash_read(fw_offset + part->size - EFV_IMAGE_METADATA_SIZE, (uint32_t *)metadata, EFV_IMAGE_METADATA_SIZE);
    ERROR_CODE_CHECK(ret != 0, "metadata read failed");
    ERROR_CODE_CHECK(EFV_MANIFEST_MAGIC != header->manifest_magic, "invalid manifest magic");
    ERROR_CODE_CHECK(EFV_MANIFEST_VERSION != header->manifest_version, "invalid manifest format");
    ERROR_CODE_CHECK(EFV_IMAGE_FORMAT_0 != header->image_format, "invalid image format");

    uint8_t *signature = metadata + EFV_MANIFEST_SIZE;
    ret = efv_ed25519_verify(pub_key, signature, metadata, EFV_MANIFEST_SIZE);
    ERROR_CODE_CHECK(ret != 0, "signature verify failed");
    ESP_LOGI(TAG, "[2/4] Signature OK");

    ERROR_CODE_CHECK(header->image_size != part->size, "invalid image size");
    ERROR_CODE_CHECK(header->payload_size == 0, "zero payload size");
    ERROR_CODE_CHECK(header->payload_size != part->size - EFV_IMAGE_METADATA_SIZE, "invalid payload size");
    ERROR_CODE_CHECK(header->firmware_size + EFV_IMAGE_METADATA_SIZE > part->size, "firmware size exceeds partition");
    ESP_LOGI(TAG, "[3/4] Manifest OK");

    uint8_t hash[EFV_SHA256_HASH_SIZE] __attribute__((aligned(4)));
    ret = bootloader_sha256_hash_stream(hash, fw_offset, header->payload_size);
    ERROR_CODE_CHECK(ret != 0, "firmware hash failed");
    ret = memcmp(header->firmware_sha256, hash, EFV_SHA256_HASH_SIZE);
    ERROR_CODE_CHECK(ret != 0, "firmware hash mismatch");
    ESP_LOGI(TAG, "[4/4] Hash OK");

    return 0;
}

void bootloader_partition_verify_and_boot(const bootloader_state_t *bs, int index)
{
    int ret = bootloader_partition_verify(bs, index);
    if (ret == 0)
    {
        ESP_LOGI(TAG, "Booting from partition %d", index);
        bootloader_utility_load_boot_image(bs, index);
        ESP_LOGE(TAG, "Partition %d boot failed", index);
    }
}

void bootloader_after_init(void)
{

    bootloader_state_t bs = {0};

    if (!bootloader_utility_load_partition_table(&bs))
    {
        ESP_LOGE(TAG, "load partition table failed");
        esp_rom_delay_us(3000000);
        bootloader_reset();
    }

    int selected = bootloader_utility_get_selected_boot_partition(&bs);
    ESP_LOGI(TAG, "Selected boot partition index=%d app_count=%d", selected, bs.app_count);

    if (selected >= 0 && selected < (int)bs.app_count)
    {
        bootloader_partition_verify_and_boot(&bs, selected);
    }

    for (int i = 0; i < (int)bs.app_count; i++)
    {
        if (i == selected)
        {
            continue;
        }
        bootloader_partition_verify_and_boot(&bs, i);
    }

    ESP_LOGE(TAG, "no valid OTA firmware, reset in 3s...");
    esp_rom_delay_us(3000000);
    bootloader_reset();
}