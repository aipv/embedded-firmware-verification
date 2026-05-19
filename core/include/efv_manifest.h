#ifndef EFV_MANIFEST_H
#define EFV_MANIFEST_H

#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================================================
 * Manifest Constants
 * ==================================================================================================== */
#define EFV_IMAGE_METADATA_SIZE    0x80u              /* manifest + signature */
#define EFV_IMAGE_FORMAT_0         0x0u               /* firmware + padding + manifest + signature */
#define EFV_IMAGE_FORMAT_1         0x1u               /* manifest + signature + firmware */

#define EFV_MANIFEST_SIZE          0x40u              /* sizeof(efv_manifest_t) */
#define EFV_MANIFEST_MAGIC         0x4654504Du        /* "FTPM" */
#define EFV_MANIFEST_VERSION       0x1u

/* ====================================================================================================
 * Manifest binary format:
 *
 * All integer fields are little-endian.
 * Total manifest size is fixed to 64 bytes.
 * ==================================================================================================== */
typedef struct
{
    uint32_t manifest_magic;
    uint32_t manifest_version;
    uint32_t image_format;
    uint32_t image_size;            /* Total image size including firmware, padding, manifest and signature */
    uint32_t payload_size;          /* firmware + padding */
    uint32_t firmware_size;
    uint32_t firmware_version;
    uint32_t reserved;           /* Reserved for future use, must be zero */
    uint8_t  firmware_sha256[32];   /* SHA256(firmware + padding), extensible in future manifest versions */
} efv_manifest_t;

_Static_assert(sizeof(efv_manifest_t) == EFV_MANIFEST_SIZE, "efv_manifest_t size must be 64 bytes");

#ifdef __cplusplus
}
#endif

#endif /* EFV_MANIFEST_H */