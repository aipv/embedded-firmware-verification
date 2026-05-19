#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sodium.h>
#include "bss_utils.h"

#define SIGNATURE_SIZE crypto_sign_BYTES        // 64
#define PUBLIC_KEY_SIZE crypto_sign_PUBLICKEYBYTES  // 32

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <public.key> <payload> <signature>\n", argv[0]);
        return -1;
    }

    if (sodium_init() < 0) {
        fprintf(stderr, "Failed to init libsodium\n");
        return -1;
    }

    const char *pubkey_path = argv[1];
    const char *payload_path = argv[2];
    const char *sig_path = argv[3];

    /* 读取公钥 */
    size_t pk_len = 0;
    uint8_t *pk = bss_read_file_to_dynamic_buffer(pubkey_path, &pk_len);
    if (!pk || pk_len != PUBLIC_KEY_SIZE) {
        fprintf(stderr, "Error: public key must be exactly 32 bytes\n");
        free(pk);
        return -1;
    }

    /* 读取 payload */
    size_t msg_len = 0;
    uint8_t *msg = bss_read_file_to_dynamic_buffer(payload_path, &msg_len);
    if (!msg) {
        fprintf(stderr, "Error: failed to read payload\n");
        free(pk);
        return -1;
    }

    /* 读取 signature */
    size_t sig_len = 0;
    uint8_t *sig = bss_read_file_to_dynamic_buffer(sig_path, &sig_len);
    if (!sig || sig_len != SIGNATURE_SIZE) {
        fprintf(stderr, "Error: signature must be exactly 64 bytes\n");
        free(pk);
        free(msg);
        free(sig);
        return -1;
    }

    /* 验证签名 */
    int ret = crypto_sign_verify_detached(
        sig,
        msg,
        msg_len,
        pk
    );

    if (ret == 0) {
        printf("✅ Signature VALID\n");
    } else {
        printf("❌ Signature INVALID\n");
    }

    free(pk);
    free(msg);
    free(sig);

    return ret;
}