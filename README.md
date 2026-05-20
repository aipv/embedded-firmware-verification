# README.md

# embedded-firmware-verification

![Build](https://github.com/aipv/embedded-firmware-verification/actions/workflows/build.yml/badge.svg)

A lightweight signed firmware verification library for resource-constrained embedded systems.

This project provides:

- Firmware SHA256 verification
- Ed25519 signature verification
- Signed firmware image validation
- Embedded-friendly cryptographic verification
- Minimal-dependency firmware verification
- ESP32-S3 verified boot integration example

The library is designed for:

- MCU-based systems
- RTOS environments
- OTA-capable devices
- Embedded secure boot verification
- Resource-constrained platforms

---

# Features

## Firmware Verification

- SHA256 firmware hashing
- Ed25519 detached signature verification
- Signed firmware validation
- Firmware authenticity checks
- Firmware integrity checks

## Embedded Design

- Lightweight C implementation
- Minimal memory footprint
- Minimal-dependency firmware verification
- Portable embedded design
- TweetNaCl verification backend

## OTA Integration

- OTA firmware authenticity validation
- Signed firmware update verification
- Corrupted firmware detection
- Tampered firmware rejection

---

# Firmware Verification Flow

```text
Firmware Payload
        ↓
SHA256(payload)
        ↓
Manifest Creation
        ↓
Ed25519 Signature
        ↓
Signed Firmware Image
        ↓
Firmware Verification
```

Verification sequence:

1. Verify Ed25519 signature
2. Validate firmware metadata
3. Recalculate firmware SHA256
4. Compare firmware digest
5. Boot verified firmware

---

# Cryptography

| Algorithm | Purpose |
|---|---|
| SHA256 | Firmware integrity verification |
| Ed25519 | Firmware authentication |
| TweetNaCl | Embedded signature verification |
| libsodium | Host-side signing tools |

---

# Build

## Requirements

### Host Tools

- CMake
- GCC or Clang
- libsodium

### Optional

- ESP-IDF
- ESP32-S3 toolchain

---

## Build Project

```bash
mkdir build
cd build
cmake ..
make
```

---

# Unit Tests

Run unit tests in build/ folder:

```bash
./test_efv_sha256
./test_efv_ed25519
```

---

# Integration Tests

Run integration tests in build/ folder:

```bash
../tests/test_signature_flow.sh
../tests/test_image_flow.sh
```

---

# ESP32-S3 Verified Boot Example

An ESP32-S3 OTA firmware verification example using the embedded-firmware-verification library.

This project demonstrates:

- Signed firmware image creation
- SHA256 firmware integrity verification
- Ed25519 firmware signature verification
- Boot-time firmware validation
- MQTT-triggered OTA firmware updates
- HTTP OTA firmware delivery
- Tampered firmware rejection

The example integrates ESP-IDF, TweetNaCl-based Ed25519 verification, OTA firmware updates, and bootloader verification flow on ESP32-S3 devices.

```text
Firmware Build
      ↓
Signed Firmware Image
      ↓
Firmware Verification
      ↓
Local Flash / OTA Update
      ↓
Bootloader Verification
      ↓
Boot Verified Firmware
```

## Requirements

- ESP32-S3 development board
- ESP-IDF
- Python 3
- libsodium
- mosquitto-clients

## Recommended Workflow

### Step 1: Build and Flash Default Firmware

```bash
cd examples/esp32-s3-secure-bootloader
idf.py build flash monitor
```

The default firmware image is rejected because it is not signed.

### Step 2: Create Signed Firmware Image and Flash

```bash
./tests/create_esp32-s3_image.sh
./tests/flash_esp32-s3_image.sh
idf.py monitor
```

The bootloader successfully verifies and boots the signed firmware image.

### Step 3: OTA Update Flow

Terminal 1: Start OTA HTTP Server

```bash
./tests/ota_server_start.sh
```

Terminal 2: Trigger OTA Update

```bash
./tests/ota_update_trigger.sh
```

The ESP32-S3 device downloads the signed firmware image and verifies the firmware during boot.

## Example Boot Verification Log

```text
I (102) EFV_BOOT_VERIFY: Selected boot partition index=1 app_count=2
I (108) EFV_BOOT_VERIFY: ========== Verify ota_1 ==========
I (117) EFV_BOOT_VERIFY: [1/4] Partition OK
I (1114) EFV_BOOT_VERIFY: [2/4] Signature OK
I (1114) EFV_BOOT_VERIFY: [3/4] Manifest OK
I (1587) EFV_BOOT_VERIFY: [4/4] Hash OK
I (1587) EFV_BOOT_VERIFY: Booting from partition 1
```

# Security Scope

Currently implemented:

- Firmware authenticity verification
- Firmware integrity verification
- Signed firmware validation
- OTA firmware verification
- Embedded Ed25519 verification

Not yet implemented:

- Immutable hardware Root of Trust
- Anti-rollback protection
- Secure key provisioning
- Image encryption
- Measured boot
- Remote attestation
- TPM/TEE integration
- Key rotation/revocation

---

# Threat Model

The framework currently protects against:

- Unauthorized firmware modification
- Corrupted firmware images
- Unsigned firmware execution
- Tampered OTA payloads

The framework currently does NOT fully protect against:

- Physical attacks
- Public key replacement
- Firmware rollback attacks
- Runtime compromise
- Side-channel attacks

---

# Design Goals

The project prioritizes:

- Simplicity
- Auditability
- Small footprint
- Embedded portability
- Minimal dependencies
- Clear verification flow

This framework is intended as:

- A lightweight firmware verification layer
- An embedded firmware authentication component
- A reference implementation for signed firmware validation

---

# Contributing

Contributions are welcome.

Recommended future additions:

- GitHub Actions CI
- Static analysis
- Fuzz testing
- Cross-platform build testing
- Security regression testing

---
