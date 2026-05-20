#!/bin/bash

set -e

APP_FW_IMAGE="build/esp32-s3-secure-boot-example.bin"

SIGNED_FW_IMAGE="signed_firmware.bin"

PRIVATE_KEY="tests/private_key"

PUBLIC_KEY="tests/public_key"

FW_SIZE_LIMIT="0x1f0000"

FW_VERSION="1"

echo "[INFO] Creating signed firmware image:"
echo "  Input : $APP_FW_IMAGE"
echo "  Output: $SIGNED_FW_IMAGE"

../../build/sodium_image_create \
    "$PRIVATE_KEY" \
    "$APP_FW_IMAGE" \
    "$SIGNED_FW_IMAGE" \
    "$FW_SIZE_LIMIT" \
    "$FW_VERSION"

echo "[INFO] Verifying signed firmware image:"
echo "  $SIGNED_FW_IMAGE"

../../build/efv_image_verify \
    "$PUBLIC_KEY" \
    "$SIGNED_FW_IMAGE"

echo "[INFO] Signed firmware image verified successfully."