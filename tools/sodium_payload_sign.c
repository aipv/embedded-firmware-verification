#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sodium.h>
#include "bss_utils.h"

#define SIGNATURE_SIZE crypto_sign_BYTES        // 64
#define PRIVATE_KEY_SIZE crypto_sign_SECRETKEYBYTES  // 64

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <private.key> <payload> <signature>\n", argv[0]);
        return 1;
    }
    const char *key_path = argv[1];
    const char *payload_path = argv[2];
    const char *sig_path = argv[3];

    int ret = sodium_init();
    if (ret < 0) {
        fprintf(stderr, "Failed to init libsodium\n");
        return 1;
    }


    size_t key_len = 0;
    uint8_t *sk = bss_read_file_to_dynamic_buffer(key_path, &key_len);
    if (!sk || key_len != PRIVATE_KEY_SIZE) {
        fprintf(stderr, "Error: private key must be exactly 64 bytes\n");
        free(sk);
        return 1;
    }

    size_t msg_len = 0;
    uint8_t *msg = bss_read_file_to_dynamic_buffer(payload_path, &msg_len);
    if (!msg) {
        fprintf(stderr, "Error: failed to read payload\n");
        free(sk);
        return 1;
    }

    uint8_t signature[SIGNATURE_SIZE];
    unsigned long long sig_len = 0;

    if (crypto_sign_detached(
            signature,
            &sig_len,
            msg,
            msg_len,
            sk) != 0) {
        fprintf(stderr, "Error: signing failed\n");
        free(sk);
        free(msg);
        return 1;
    }

    if (sig_len != SIGNATURE_SIZE) {
        fprintf(stderr, "Error: unexpected signature size\n");
        free(sk);
        free(msg);
        return 1;
    }

    ret = bss_write_binary_file(sig_path, (const char *)signature, SIGNATURE_SIZE);
    if (ret != SIGNATURE_SIZE) {
        fprintf(stderr, "Error: failed to write signature\n");
        free(sk);
        free(msg);
        return 1;
    }

    printf("Signature generated: %s\n", sig_path);
    bss_print_hex_data("Signature : ", signature, 64, 64);

    free(sk);
    free(msg);
    return 0;
}