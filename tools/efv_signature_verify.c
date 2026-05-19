#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "bss_utils.h"
#include "efv_ed25519.h"

int main(int argc, char** argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <public.key> <payload> <signature>\n", argv[0]);
        return -1;
    }

    // 读取公钥
    size_t pk_len;
    unsigned char* pk = bss_read_file_to_dynamic_buffer(argv[1], &pk_len);
    if (!pk || pk_len != EFV_ED25519_PUBLIC_KEY_SIZE) {
        printf("Public key must be 32 bytes\n");
        return -1;
    }

    size_t msg_len;
    unsigned char* msg = bss_read_file_to_dynamic_buffer(argv[2], &msg_len);
    if (!msg) {
        printf("Failed to read message\n");
        return -1;
    }

    if (msg_len > EFV_ED25519_MAX_PAYLOAD_SIZE){
        printf("Invalid Payload Size\n");
        return -1;
    }

    size_t sig_len;
    unsigned char* sig = bss_read_file_to_dynamic_buffer(argv[3], &sig_len);
    if (!sig || sig_len != EFV_ED25519_SIGNATURE_SIZE) {
        printf("Signature must be 64 bytes\n");
        return -1;
    }

    int ret = efv_ed25519_verify(pk, sig, msg, msg_len);
    if (ret == 0) {
        printf("✅ Signature VALID\n");
    } else {
        printf("❌ Signature INVALID\n");
    }

    free(pk);
    free(sig);
    free(msg);
    return ret;
}