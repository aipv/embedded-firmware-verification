#!/bin/bash

set -e

PORT=${PORT:-/dev/ttyUSB0}

APP_FW_IMAGE="build/esp32-s3-secure-boot-example.bin"

SIGNED_FW_IMAGE=${1:-build/signed_firmware.bin}

if [ ! -f "$APP_FW_IMAGE" ]; then
    echo "[ERROR] Application firmware not found:"
    echo "  $APP_FW_IMAGE"
    exit 1
fi

echo "[INFO] Signing firmware:"
echo "  $SIGNED_FW_IMAGE"

../../build/sodium_image_create \
    tests/private_key \
    "$APP_FW_IMAGE" \
    "$SIGNED_FW_IMAGE" \
    0x1f0000 \
    1

echo "[INFO] Verifying firmware:"
echo "  $SIGNED_FW_IMAGE"

../../build/efv_image_verify \
    tests/public_key \
    "$SIGNED_FW_IMAGE"

echo "[INFO] Flashing signed firmware:"
echo "  $SIGNED_FW_IMAGE"

esptool.py \
    --chip esp32s3 \
    --port "$PORT" \
    write_flash \
    0x20000 "$SIGNED_FW_IMAGE"

echo "[INFO] Flash completed."