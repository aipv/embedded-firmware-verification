#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "efv_ed25519.h"

/* ====================================================================================================
 * Test Vectors
 * ==================================================================================================== */

static const uint8_t test_public_key[32] =
{
    0x20, 0x4B, 0x05, 0xB6, 0xD3, 0x38, 0x1A, 0x35,
    0x84, 0x61, 0xE8, 0x16, 0x0F, 0xC9, 0x2D, 0xCB,
    0x4C, 0x47, 0x80, 0x67, 0x51, 0x82, 0x92, 0xF5,
    0x1F, 0x7A, 0x9C, 0x1F, 0xDA, 0x66, 0xF8, 0xDE
};

static const uint8_t test_signature[64] =
{
    0x9F, 0x6F, 0xBE, 0x62, 0x33, 0xAF, 0xA6, 0xDF,
    0x9D, 0x6B, 0x36, 0x9F, 0x85, 0x26, 0x88, 0xB9,
    0x4A, 0x29, 0x68, 0x49, 0x94, 0xE0, 0xCC, 0x4B,
    0xB9, 0xA3, 0x31, 0x04, 0xB1, 0x5E, 0xA8, 0x51,
    0x6A, 0xA9, 0xFB, 0x3B, 0x68, 0xDD, 0x31, 0x71,
    0x77, 0xE2, 0xF6, 0x17, 0xD5, 0x7A, 0x4B, 0xC1,
    0x90, 0x7D, 0x69, 0xAA, 0x70, 0xD2, 0xE5, 0x4F,
    0x9C, 0x97, 0x0B, 0x19, 0x8B, 0xAB, 0xCF, 0x08
};

static const uint8_t test_payload[64] =
{
    0x9D, 0xEC, 0x49, 0xFA, 0x38, 0x7E, 0xAF, 0x2A,
    0x9F, 0xD1, 0xAF, 0x08, 0xFA, 0x4D, 0x61, 0x7B,
    0x39, 0x54, 0xB3, 0x14, 0xFC, 0x59, 0xFA, 0x0A,
    0x8D, 0xDF, 0x8D, 0x32, 0x6C, 0x73, 0x5C, 0x34,
    0x30, 0x3A, 0xBD, 0xAC, 0xF0, 0x75, 0xEE, 0x75,
    0x8A, 0xA3, 0xDA, 0x07, 0xBD, 0xA1, 0x96, 0xD9,
    0xF5, 0x4C, 0x82, 0x4E, 0x98, 0xA4, 0x24, 0x80,
    0xF3, 0xBD, 0x11, 0x40, 0x20, 0x53, 0xF1, 0x7E
};

/* ====================================================================================================
 * Tests
 * ==================================================================================================== */

static void test_valid_signature(void)
{
    int ret;

    ret = efv_ed25519_verify(test_public_key,
                             test_signature,
                             test_payload,
                             sizeof(test_payload));

    assert(ret == EFV_ED25519_OK);

    printf("[PASS] valid signature\n");
}

static void test_modified_payload(void)
{
    uint8_t payload[sizeof(test_payload)];
    int ret;

    memcpy(payload, test_payload, sizeof(payload));

    payload[0] ^= 0xFF;

    ret = efv_ed25519_verify(test_public_key,
                             test_signature,
                             payload,
                             sizeof(payload));

    assert(ret == EFV_ED25519_VERIFY_FAILED);

    printf("[PASS] modified payload\n");
}

static void test_modified_signature(void)
{
    uint8_t signature[sizeof(test_signature)];
    int ret;

    memcpy(signature, test_signature, sizeof(signature));

    signature[0] ^= 0xAA;

    ret = efv_ed25519_verify(test_public_key,
                             signature,
                             test_payload,
                             sizeof(test_payload));

    assert(ret == EFV_ED25519_VERIFY_FAILED);

    printf("[PASS] modified signature\n");
}

static void test_modified_public_key(void)
{
    uint8_t public_key[sizeof(test_public_key)];
    int ret;

    memcpy(public_key, test_public_key, sizeof(public_key));

    public_key[0] ^= 0x55;

    ret = efv_ed25519_verify(public_key,
                             test_signature,
                             test_payload,
                             sizeof(test_payload));

    assert(ret == EFV_ED25519_VERIFY_FAILED);

    printf("[PASS] modified public key\n");
}

static void test_null_parameters(void)
{
    uint8_t dummy[1] = {0};

    assert(efv_ed25519_verify(NULL,
                              dummy,
                              dummy,
                              1) == EFV_ED25519_INVALID_PARAM);

    assert(efv_ed25519_verify(dummy,
                              NULL,
                              dummy,
                              1) == EFV_ED25519_INVALID_PARAM);

    assert(efv_ed25519_verify(dummy,
                              dummy,
                              NULL,
                              1) == EFV_ED25519_INVALID_PARAM);

    printf("[PASS] null parameters\n");
}

static void test_zero_size_payload(void)
{
    uint8_t signature[EFV_ED25519_SIGNATURE_SIZE] = {0};
    uint8_t public_key[EFV_ED25519_PUBLIC_KEY_SIZE] = {0};

    int ret;

    ret = efv_ed25519_verify(public_key,
                             signature,
                             NULL,
                             0);
    assert(ret == EFV_ED25519_INVALID_PARAM);

    printf("[PASS] zero size payload\n");
}

int main(void)
{
    test_valid_signature();

    test_modified_payload();

    test_modified_signature();

    test_modified_public_key();

    test_null_parameters();

    test_zero_size_payload();

    printf("\nAll efv_ed25519 tests passed.\n");

    return 0;
}