#ifndef EFV_SHA256_H
#define EFV_SHA256_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================================================
 * SHA-256 Constants
 * ==================================================================================================== */
#define EFV_SHA256_HASH_SIZE         32u

/* ====================================================================================================
 * SHA-256 Structure
 * ==================================================================================================== */
typedef struct {
    uint32_t state[8];
    uint64_t bitlen;
    uint8_t  buffer[64];
    uint32_t buffer_len;
} efv_sha256_ctx;

void efv_sha256_init(efv_sha256_ctx *ctx);
void efv_sha256_update(efv_sha256_ctx *ctx, const uint8_t *data, uint32_t len);
void efv_sha256_final(efv_sha256_ctx *ctx, uint8_t hash[EFV_SHA256_HASH_SIZE]);
int efv_sha256_hash(uint8_t hash[EFV_SHA256_HASH_SIZE], const uint8_t *data, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* EFV_SHA256_H */