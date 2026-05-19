#ifndef EFV_ED25519_H
#define EFV_ED25519_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================================================
 * ED25519 Constants
 * ==================================================================================================== */
#define EFV_ED25519_PRIVATE_KEY_SIZE    64
#define EFV_ED25519_PUBLIC_KEY_SIZE     32
#define EFV_ED25519_SIGNATURE_SIZE      64
#define EFV_ED25519_MAX_PAYLOAD_SIZE    64

/* ====================================================================================================
 * Error Codes
 * ==================================================================================================== */
#define EFV_ED25519_OK                   0
#define EFV_ED25519_VERIFY_FAILED       -1
#define EFV_ED25519_INVALID_PARAM       -10
#define EFV_ED25519_ERROR               -100


/* ====================================================================================================
 * Verify Ed25519 Signature using tweetnacl wrapper
 *
 * ram = R || A || M
 *   [0..31]   R : signature point
 *   [32..63]  A : public key
 *   [64..end] M : message
 *
 * ram_len : msg_len + 64
 *
 * s = signature scalar
 *
 * Returns:
 *    0  valid
 *   -1  invalid
 *
 * ==================================================================================================== */
int efv_ed25519_tweetnacl_verify(const uint8_t *ram, uint32_t ram_len, const uint8_t s[32]);

/* ====================================================================================================
 * Verify Ed25519 Signature
 *
 * Verifies an Ed25519 signature over a message.
 *
 * Parameters:
 *   public_key  - Ed25519 public key, must be EFV_ED25519_PUBLIC_KEY_SIZE bytes
 *   signature   - Ed25519 signature, must be EFV_ED25519_SIGNATURE_SIZE bytes
 *   data        - message buffer; may be NULL only when size is 0
 *   size        - message length in bytes
 *
 * Return:
 *   EFV_ED25519_OK on success
 *   EFV_ED25519_VERIFY_FAILED when signature verification fails
 *   EFV_ED25519_INVALID_PARAM on invalid input
 *   EFV_ED25519_ERROR on internal error
 * ==================================================================================================== */
int efv_ed25519_verify(const uint8_t *public_key, const uint8_t *signature, const uint8_t *data, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* EFV_ED25519_H */