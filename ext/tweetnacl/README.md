# TweetNaCl Integration

This directory contains the upstream TweetNaCl implementation used by Firmware Trust Framework.

## Source

TweetNaCl version: 20140427

Official source:
[https://tweetnacl.cr.yp.to/software.html](https://tweetnacl.cr.yp.to/software.html)

Files:

* tweetnacl.c
* tweetnacl.h

## Purpose

TweetNaCl is used as the cryptographic backend for:

* Ed25519 signature verification

within the secure firmware verification flow.

## Integration Policy

The upstream TweetNaCl source is intentionally preserved with minimal modification.

Project-specific logic is implemented outside of TweetNaCl through wrapper interfaces.

This approach improves:

* auditability
* maintainability
* upstream comparison
* cryptographic implementation stability

## Notes

Current project usage focuses on:

* Ed25519 verification

Future versions may introduce:

* verify-only reduction
* size optimization
* embedded-specific integration improvements

---

# LICENSE

TweetNaCl version 20140427
Source: [https://tweetnacl.cr.yp.to/software.html](https://tweetnacl.cr.yp.to/software.html)

Author:
Daniel J. Bernstein and contributors

TweetNaCl is placed in the public domain.

To the extent possible under law, the author has waived all copyright
and related or neighboring rights to TweetNaCl.

You may use, modify, distribute, and reproduce TweetNaCl freely.
