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
