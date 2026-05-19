#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="."

KEY_PUB="${BUILD_DIR}/test_pub.key"
KEY_PRIV="${BUILD_DIR}/test_priv.key"
PAYLOAD="${BUILD_DIR}/test_payload.bin"
SIGNATURE="${BUILD_DIR}/test_payload.sig"
BAD_PAYLOAD="${BUILD_DIR}/test_payload_bad.bin"
BAD_SIGNATURE="${BUILD_DIR}/test_payload_bad.sig"

echo "[INFO] Using build dir: ${BUILD_DIR}"

required_tools=(
  "sodium_keypair_create"
  "sodium_payload_sign"
  "sodium_signature_verify"
  "tweetnacl_signature_verify"
  "efv_signature_verify"
)

for tool in "${required_tools[@]}"; do
    if [[ ! -x "${BUILD_DIR}/${tool}" ]]; then
        echo "[ERROR] Missing executable: ${BUILD_DIR}/${tool}"
        exit 1
    fi
done

echo "[TEST] Create payload"
cp ../tests/test_payload.bin ${PAYLOAD}

echo "[TEST] Generate Ed25519 keypair"
"${BUILD_DIR}/sodium_keypair_create" "${KEY_PUB}" "${KEY_PRIV}"

echo "[TEST] Sign payload with libsodium"
"${BUILD_DIR}/sodium_payload_sign" "${KEY_PRIV}" "${PAYLOAD}" "${SIGNATURE}"

echo "[TEST] Verify signature with libsodium"
"${BUILD_DIR}/sodium_signature_verify" "${KEY_PUB}" "${PAYLOAD}" "${SIGNATURE}"

echo "[TEST] Verify signature with TweetNaCl"
"${BUILD_DIR}/tweetnacl_signature_verify" "${KEY_PUB}" "${PAYLOAD}" "${SIGNATURE}"

echo "[TEST] Verify signature with FTP"
"${BUILD_DIR}/efv_signature_verify" "${KEY_PUB}" "${PAYLOAD}" "${SIGNATURE}"

echo "[TEST] Tamper payload and expect verification failure"
cp "${PAYLOAD}" "${BAD_PAYLOAD}"
printf "tamper" >> "${BAD_PAYLOAD}"

if "${BUILD_DIR}/sodium_signature_verify" "${KEY_PUB}" "${BAD_PAYLOAD}" "${SIGNATURE}"; then
    echo "[ERROR] libsodium accepted tampered payload"
    exit 1
else
    echo "[OK] libsodium rejected tampered payload"
fi

if "${BUILD_DIR}/tweetnacl_signature_verify" "${KEY_PUB}" "${BAD_PAYLOAD}" "${SIGNATURE}"; then
    echo "[ERROR] TweetNaCl accepted tampered payload"
    exit 1
else
    echo "[OK] TweetNaCl rejected tampered payload"
fi

if "${BUILD_DIR}/efv_signature_verify" "${KEY_PUB}" "${BAD_PAYLOAD}" "${SIGNATURE}"; then
    echo "[ERROR] FTP accepted tampered payload"
    exit 1
else
    echo "[OK] FTP rejected tampered payload"
fi

echo "[TEST] Tamper signature and expect verification failure"
cp "${SIGNATURE}" "${BAD_SIGNATURE}"
printf "\x00" | dd of="${BAD_SIGNATURE}" bs=1 seek=0 count=1 conv=notrunc status=none

if "${BUILD_DIR}/sodium_signature_verify" "${KEY_PUB}" "${PAYLOAD}" "${BAD_SIGNATURE}"; then
    echo "[ERROR] libsodium accepted tampered signature"
    exit 1
else
    echo "[OK] libsodium rejected tampered signature"
fi

if "${BUILD_DIR}/tweetnacl_signature_verify" "${KEY_PUB}" "${PAYLOAD}" "${BAD_SIGNATURE}"; then
    echo "[ERROR] TweetNaCl accepted tampered signature"
    exit 1
else
    echo "[OK] TweetNaCl rejected tampered signature"
fi

if "${BUILD_DIR}/efv_signature_verify" "${KEY_PUB}" "${PAYLOAD}" "${BAD_SIGNATURE}"; then
    echo "[ERROR] FTP accepted tampered signature"
    exit 1
else
    echo "[OK] FTP rejected tampered signature"
fi

echo "[PASS] Signature flow test completed successfully"