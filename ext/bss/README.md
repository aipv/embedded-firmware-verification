# Base System Support Utilities

A lightweight utility library for C projects.

`bss_utils` provides small and reusable helper functions for:

- File I/O
- Binary/Text file handling
- Debug logging
- Error checking macros
- Hex dump utilities

Designed for:

- Linux applications
- Embedded systems
- Middleware utilities
- Small native C projects

---

# Features

## File Utilities

- Read binary files
- Write binary files
- Read text files
- Write text files
- Load entire file into dynamic memory

## Debugging Helpers

- Optional logging macros
- Hex dump printer

## Error Handling Macros

Simple and readable guard macros:

```c
BSS_RETURN_IF_NULL(ptr, err);
BSS_RETURN_IF_TRUE(cond, err);
BSS_RETURN_IF_FALSE(cond, err);