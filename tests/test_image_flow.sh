#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="."
FIRMWARE="${2:-../tests/test_firmware.bin}"

PRIV_KEY="${BUILD_DIR}/test_priv.key"
PUB_KEY="${BUILD_DIR}/test_pub.key"
IMAGE="${BUILD_DIR}/image.bin"

IMAGE_BASE="0x200000"
IMAGE_VERSION="1"

echo "[INFO] build dir : ${BUILD_DIR}"
echo "[INFO] firmware  : ${FIRMWARE}"

for tool in sodium_keypair_create sodium_image_create efv_image_verify; do
    if [[ ! -x "${BUILD_DIR}/${tool}" ]]; then
        echo "[ERROR] missing executable: ${BUILD_DIR}/${tool}"
        exit 1
    fi
done

if [[ ! -f "${FIRMWARE}" ]]; then
    echo "[ERROR] firmware not found: ${FIRMWARE}"
    exit 1
fi

echo "[TEST] generate keypair"
"${BUILD_DIR}/sodium_keypair_create" "${PUB_KEY}" "${PRIV_KEY}"

echo "[TEST] create trusted firmware image"
"${BUILD_DIR}/sodium_image_create" \
    "${PRIV_KEY}" \
    "${FIRMWARE}" \
    "${IMAGE}" \
    "${IMAGE_BASE}" \
    "${IMAGE_VERSION}"

if [[ ! -f "${IMAGE}" ]]; then
    echo "[ERROR] image was not created: ${IMAGE}"
    exit 1
fi

echo "[TEST] verify trusted firmware image"
VERIFY_OUTPUT="$("${BUILD_DIR}/efv_image_verify" "${PUB_KEY}" "${IMAGE}")"
echo "${VERIFY_OUTPUT}"

if ! grep -q "=== VERIFICATION SUCCESS ===" <<< "${VERIFY_OUTPUT}"; then
    echo "[ERROR] image verification failed"
    exit 1
fi

echo "[PASS] image create + verify flow completed successfully"
