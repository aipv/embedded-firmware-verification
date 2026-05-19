#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "bss_utils.h"
#include "tweetnacl.c"

#define PK_SIZE 32
#define SIG_SIZE 64
#define RAM_BUF_SIZE 0x200000

uint8_t ram_buf[RAM_BUF_SIZE];

void randombytes(u8 *buf, u64 len){};

/*
 * Verify Ed25519 signature
 *
 * ram = R || A || M
 *   R : signature point
 *   A : public key
 *   M : message
 *
 * ram_len : msg_len + 64
 *
 * s = signature scalar
 *
 * Returns:
 *    0  valid
 *   -1  invalid
 */
int crypto_sign64_msg64_verify(const uint8_t *ram, uint32_t ram_len, const uint8_t s[32])
{
    u8 h[64], t[32];
    gf p[4], q[4];

    if (unpackneg(q, ram + 32))
        return -1;
    crypto_hash(h, ram, ram_len);
    reduce(h);

    scalarmult(p, q, h);
    scalarbase(q, s);
    add(p, q);
    pack(t, p);

    if (crypto_verify_32(ram, t))
        return -1;

    return 0;
}

int my_sign64_msg64_verify(const uint8_t pk[32], const uint8_t sig[64], const uint8_t *msg, uint32_t msg_len)
{
    memcpy(ram_buf, sig, 32);
    memcpy(ram_buf + 32, pk, 32);
    memcpy(ram_buf + 64, msg, msg_len);
    int ret = crypto_sign64_msg64_verify(ram_buf, msg_len + 64, sig + 32);
    return ret;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <public.key> <payload> <signature>\n", argv[0]);
        return -1;
    }

    // 读取公钥
    size_t pk_len;
    unsigned char* pk = bss_read_file_to_dynamic_buffer(argv[1], &pk_len);
    if (!pk || pk_len != PK_SIZE) {
        printf("Public key must be 32 bytes\n");
        return -1;
    }

    size_t msg_len;
    unsigned char* msg = bss_read_file_to_dynamic_buffer(argv[2], &msg_len);
    if (!msg) {
        printf("Failed to read message\n");
        return -1;
    }

    if (msg_len + 64 > RAM_BUF_SIZE){
        printf("Invalid Payload Size\n");
        return -1;
    }

    size_t sig_len;
    unsigned char* sig = bss_read_file_to_dynamic_buffer(argv[3], &sig_len);
    if (!sig || sig_len != SIG_SIZE) {
        printf("Signature must be 64 bytes\n");
        return -1;
    }

    // 构造 sm = sig || msg
    size_t sm_len = SIG_SIZE + msg_len;
    unsigned char* sm = malloc(sm_len);
    if (!sm) return -1;

    memcpy(sm, sig, SIG_SIZE);
    memcpy(sm + SIG_SIZE, msg, msg_len);

    // 输出 buffer
    unsigned char* m_out = malloc(sm_len);
    unsigned long long mlen_out;

    int ret = crypto_sign_ed25519_tweet_open(m_out, &mlen_out, sm, sm_len, pk);

    if (ret == 0) {
        printf("✅ Signature VALID\n");
    } else {
        printf("❌ Signature INVALID\n");
    }

    ret = my_sign64_msg64_verify(pk, sig, msg, msg_len);
    if (ret == 0) {
        printf("✅ Signature VALID\n");
    } else {
        printf("❌ Signature INVALID\n");
    }

    // 清理
    free(pk);
    free(sig);
    free(msg);
    free(sm);
    free(m_out);

    return ret;
}