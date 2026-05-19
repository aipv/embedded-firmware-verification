#include <string.h>
#include "efv_ed25519.h"

#define EFV_ED25519_RAM_BUF_SIZE (EFV_ED25519_MAX_PAYLOAD_SIZE + 64)

int efv_ed25519_verify(const uint8_t *public_key, const uint8_t *signature, const uint8_t *data, size_t size)
{
    uint8_t ram_buf[EFV_ED25519_RAM_BUF_SIZE];

    if (public_key == NULL || signature == NULL || data == NULL || size == 0)
    {
        return EFV_ED25519_INVALID_PARAM;
    }

    if (EFV_ED25519_MAX_PAYLOAD_SIZE < size)
    {
        return EFV_ED25519_INVALID_PARAM;
    }

    memcpy(ram_buf, signature, 32);
    memcpy(ram_buf + 32, public_key, 32);
    memcpy(ram_buf + 64, data, size);
    int ret = efv_ed25519_tweetnacl_verify(ram_buf, size + 64, signature + 32);
    return ret;
}

