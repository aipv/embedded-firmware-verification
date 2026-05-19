#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "efv_sha256.h"

static void print_hash(const uint8_t hash[EFV_SHA256_HASH_SIZE])
{
    for (uint32_t i = 0; i < EFV_SHA256_HASH_SIZE; i++) {
        printf("%02x", hash[i]);
    }

    printf("\n");
}

static int hash_equal(const uint8_t *a, const uint8_t *b)
{
    return (memcmp(a, b, EFV_SHA256_HASH_SIZE) == 0);
}

int main(void)
{
    uint8_t hash[EFV_SHA256_HASH_SIZE];

    static const uint8_t expected_empty[EFV_SHA256_HASH_SIZE] = {
        0xe3,0xb0,0xc4,0x42,0x98,0xfc,0x1c,0x14,
        0x9a,0xfb,0xf4,0xc8,0x99,0x6f,0xb9,0x24,
        0x27,0xae,0x41,0xe4,0x64,0x9b,0x93,0x4c,
        0xa4,0x95,0x99,0x1b,0x78,0x52,0xb8,0x55
    };

    static const uint8_t expected_abc[EFV_SHA256_HASH_SIZE] = {
        0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,
        0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,
        0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,
        0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad
    };

    printf("==== SHA256 TEST ====\n");

    /*
     * Test vector: empty string
     */
    if (efv_sha256_hash(hash, NULL, 0) != 0) {
        printf("[FAIL] empty string hash failed\n");
        return -1;
    }

    printf("[INFO] SHA256(\"\") = ");
    print_hash(hash);

    if (!hash_equal(hash, expected_empty)) {
        printf("[FAIL] empty string mismatch\n");
        return -1;
    }

    printf("[PASS] empty string\n");

    /*
     * Test vector: "abc"
     */
    const uint8_t abc[] = "abc";

    if (efv_sha256_hash(hash, abc, 3) != 0) {
        printf("[FAIL] abc hash failed\n");
        return -1;
    }

    printf("[INFO] SHA256(\"abc\") = ");
    print_hash(hash);

    if (!hash_equal(hash, expected_abc)) {
        printf("[FAIL] abc mismatch\n");
        return -1;
    }

    printf("[PASS] abc\n");

    printf("[PASS] all SHA256 tests passed\n");

    return 0;
}