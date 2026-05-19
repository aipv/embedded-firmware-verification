#ifndef BSS_UTILS_H
#define BSS_UTILS_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================================================
 * BSS LOG
 * ==================================================================================================== */

//#define BSS_ENABLE_LOG
/* ========================= Logging ========================= */
#ifdef BSS_ENABLE_LOG
    #define BSS_LOG(...) \
        do { \
            fprintf(stderr, "[ERROR] %s:%d: ", __FILE__, __LINE__); \
            fprintf(stderr, __VA_ARGS__); \
        } while (0)
#else
    #define BSS_LOG(...)
#endif

/* ====================================================================================================
 * BSS Error Handling
 * ==================================================================================================== */
#define BSS_RETURN_IF_NULL(ptr, err) \
    do { \
        if ((ptr) == NULL) { \
            BSS_LOG("NULL pointer (err=%d)\n", err); \
            return (err); \
        } \
    } while (0)

#define BSS_RETURN_IF_NONZERO(expr, err) \
    do { \
        int _ret = (expr); \
        if (_ret != 0) { \
            BSS_LOG("expr failed (%d), err=%d\n", _ret, err); \
            return (err); \
        } \
    } while (0)

#define BSS_RETURN_IF_TRUE(expr, err) \
    do { \
        if ((expr)) { \
            BSS_LOG("condition is true (err=%d)\n", err); \
            return (err); \
        } \
    } while (0)

#define BSS_RETURN_IF_FALSE(expr, err) \
    do { \
        if (!(expr)) { \
            BSS_LOG("condition is false (err=%d)\n", err); \
            return (err); \
        } \
    } while (0)

#define BSS_CHECK(cond, msg) \
    do { \
        if (!(cond)) { \
            fprintf(stderr, "[FAIL] %s (%s:%d)\n", msg, __FILE__, __LINE__); \
            return -1; \
        } else { \
            printf("[OK] %s\n", msg); \
        } \
    } while (0)

/* ====================================================================================================
 * BSS File Handling
 * ==================================================================================================== */
int bss_read_binary_file(const char *filename, void *data, int max_size);
int bss_write_binary_file(const char *filename, const void *data, int size);
ssize_t bss_read_text_file(const char *filename, char *text, size_t max_size);
ssize_t bss_write_text_file(const char *filename, const char *text);
uint8_t* bss_read_file_to_dynamic_buffer(const char *path, size_t *size);

/* ====================================================================================================
 * BSS Data Printing
 * ==================================================================================================== */
void bss_print_hex_data(const char *name, const uint8_t *buf, size_t len, size_t max_show);

#ifdef __cplusplus
}
#endif

#endif /* BSS_UTILS_H */