#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "bss_utils.h"
#include "efv_sha256.h"
#include "efv_ed25519.h"
#include "efv_manifest.h"

#define IMAGE_BUFFER_SIZE           0x200000

uint8_t pub_key[EFV_ED25519_PUBLIC_KEY_SIZE];
uint8_t calc_hash[EFV_SHA256_HASH_SIZE];
uint8_t image_buffer[IMAGE_BUFFER_SIZE];

int efv_verify_image(uint8_t *public_key, uint8_t *image, uint32_t image_size)
{
    uint32_t payload_size = image_size - EFV_IMAGE_METADATA_SIZE;
    uint8_t *metadata = image + payload_size;
    uint8_t *signature = metadata + EFV_MANIFEST_SIZE;
    int ret = efv_ed25519_verify(public_key, signature, metadata, EFV_MANIFEST_SIZE);
    if (ret != 0) {
        fprintf(stderr, "Signature verification FAILED\n");
        return -1;
    }
    printf("Signature verification OK\n");

    efv_manifest_t *header = (efv_manifest_t *)metadata;
    BSS_CHECK(EFV_MANIFEST_MAGIC == header->manifest_magic, "manifest magic OK");
    BSS_CHECK(EFV_MANIFEST_VERSION == header->manifest_version, "manifest format OK");
    BSS_CHECK(EFV_IMAGE_FORMAT_0 == header->image_format, "image format OK");
    printf("Manifest verification OK\n");

    ret = efv_sha256_hash(calc_hash, image, payload_size);
    BSS_CHECK(ret == 0, "sha256 hash");
    ret = memcmp(calc_hash, header->firmware_sha256, EFV_SHA256_HASH_SIZE);
    BSS_CHECK(ret == 0, "sha256 hash matched");
    printf("=== VERIFICATION SUCCESS ===\n");
    return ret;
}


int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <public_key> <image>\n", argv[0]);
        return 1;
    }
    const char *key_path = argv[1];
    const char *image_path = argv[2];

    int ret = bss_read_binary_file(key_path, pub_key, EFV_ED25519_PUBLIC_KEY_SIZE);
    BSS_CHECK(ret == EFV_ED25519_PUBLIC_KEY_SIZE, "read public key");

    int image_size = bss_read_binary_file(image_path, image_buffer, IMAGE_BUFFER_SIZE);
    BSS_CHECK(EFV_IMAGE_METADATA_SIZE < image_size, "read image");

    ret = efv_verify_image(pub_key, image_buffer, image_size);
    return ret;
}