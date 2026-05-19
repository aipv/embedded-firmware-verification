#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sodium.h>
#include "bss_utils.h"
#include "efv_ed25519.h"
#include "efv_manifest.h"

#define IMAGE_BUFFER_SIZE           0x200000

uint8_t firmware_image[IMAGE_BUFFER_SIZE];
uint8_t priv_key[EFV_ED25519_PRIVATE_KEY_SIZE];


int sodium_create_image(uint8_t *private_key, uint8_t *image_buffer, uint32_t image_size, uint32_t firmware_size, uint32_t firmware_version)
{
    uint32_t payload_size = image_size - EFV_IMAGE_METADATA_SIZE;
    uint8_t *metadata = image_buffer + payload_size;
    uint8_t *signature = metadata + EFV_MANIFEST_SIZE;
    unsigned long long sig_len = 0;

    efv_manifest_t *header = (efv_manifest_t *)(metadata);
    header->manifest_magic = EFV_MANIFEST_MAGIC;
    header->manifest_version = EFV_MANIFEST_VERSION;
    header->image_format = EFV_IMAGE_FORMAT_0;
    header->image_size = image_size;
    header->payload_size = image_size - EFV_IMAGE_METADATA_SIZE;
    header->firmware_size = firmware_size;
    header->firmware_version = firmware_version;

    int ret = sodium_init();
    BSS_CHECK(ret >= 0, "sodium_init");
    ret = crypto_hash_sha256(header->firmware_sha256, image_buffer, payload_size);
    BSS_CHECK(ret == 0, "crypto_hash_sha256");

    ret = crypto_sign_detached(signature, &sig_len, metadata, EFV_MANIFEST_SIZE, private_key);
    BSS_CHECK(ret == 0, "manifest sign");

    bss_print_hex_data("Hash : ", header->firmware_sha256, 32, 32);
    bss_print_hex_data("Metadata : ", metadata, EFV_MANIFEST_SIZE, EFV_MANIFEST_SIZE);
    bss_print_hex_data("Signature : ", signature, EFV_MANIFEST_SIZE, EFV_MANIFEST_SIZE);

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <private_key> <firmware> <image> [image_size] [firmware_version]\n", argv[0]);
        return 1;
    }

    const char *key_path = argv[1];
    const char *fw_path = argv[2];
    const char *image_path = argv[3];
    int image_size = (argc > 4) ? strtol(argv[4], NULL, 0) : IMAGE_BUFFER_SIZE;
    int fw_version = (argc > 5) ? atoi(argv[5]) : 1;
    BSS_CHECK(image_size <= IMAGE_BUFFER_SIZE, "image size");

    int ret = bss_read_binary_file(key_path, priv_key, EFV_ED25519_PRIVATE_KEY_SIZE);
    BSS_CHECK(ret == EFV_ED25519_PRIVATE_KEY_SIZE, "read private key");

    memset(firmware_image, 0, image_size);
    int fw_size = bss_read_binary_file(fw_path, firmware_image, IMAGE_BUFFER_SIZE);
    printf("Read Firmware file : %d\n", fw_size);
    BSS_CHECK(fw_size > 0 && (fw_size + EFV_IMAGE_METADATA_SIZE) <= image_size, "invalid firmware size");

    ret = sodium_create_image(priv_key, firmware_image, image_size, fw_size, fw_version);
    BSS_CHECK(ret == 0, "create image");

    ret = bss_write_binary_file(image_path, firmware_image, image_size);
    BSS_CHECK(ret > 0, "write file");
}