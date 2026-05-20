#!/bin/bash

set -e

PORT=${PORT:-8080}

OTA_IMAGE=${1:-build/signed_firmware.bin}

if [ ! -f "$OTA_IMAGE" ]; then
    echo "[ERROR] OTA image not found:"
    echo "  $OTA_IMAGE"
    exit 1
fi

OTA_DIR=$(dirname "$OTA_IMAGE")

echo "[INFO] Starting OTA HTTP server"
echo "[INFO] Port : $PORT"
echo "[INFO] Image: $(basename "$OTA_IMAGE")"

cd "$OTA_DIR"

python3 -m http.server "$PORT"
