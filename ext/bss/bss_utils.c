#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "bss_utils.h"

uint8_t* bss_read_file_to_dynamic_buffer(const char *path, size_t *size)
{
    FILE *f = fopen(path, "rb");
    if (!f) {
        perror("fopen");
        return NULL;
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return NULL;
    }

    long len = ftell(f);
    if (len < 0) {
        fclose(f);
        return NULL;
    }

    rewind(f);

    uint8_t *buf = (uint8_t*)malloc((size_t)len);
    if (!buf) {
        fclose(f);
        return NULL;
    }

    size_t read_len = fread(buf, 1, (size_t)len, f);
    fclose(f);

    if (read_len != (size_t)len) {
        free(buf);
        return NULL;
    }

    *size = (size_t)len;
    return buf;
}


int bss_read_binary_file(const char *filename, void *data, int max_size)
{
    if (!filename || !data || max_size == 0)
    {
        errno = EINVAL;
        return -1;
    }

    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        return -1;  // errno 已由 fopen 设置
    }

    int total = fread(data, 1, max_size, fp);

    if (ferror(fp))
    {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return total;
}

int bss_write_binary_file(const char *filename, const void *data, int size)
{
    if (!filename || !data)
    {
        errno = EINVAL;
        return -1;
    }

    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        return -1;
    }

    int total = fwrite(data, 1, size, fp);

    if (total != size)
    {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return total;
}

/* =========================
 * 文本文件读取
 * ========================= */
ssize_t bss_read_text_file(const char *filename, char *text, size_t max_size)
{
    if (!filename || !text || max_size == 0)
    {
        errno = EINVAL;
        return -1;
    }

    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        return -1;
    }

    /* 保证留一个 '\0' */
    size_t read_size = fread(text, 1, max_size - 1, fp);

    if (ferror(fp))
    {
        fclose(fp);
        return -1;
    }

    text[read_size] = '\0';

    fclose(fp);
    return (ssize_t)read_size;
}

/* =========================
 * 文本文件写入
 * ========================= */
ssize_t bss_write_text_file(const char *filename, const char *text)
{
    if (!filename || !text)
    {
        errno = EINVAL;
        return -1;
    }

    FILE *fp = fopen(filename, "w");
    if (!fp)
    {
        return -1;
    }

    size_t len = strlen(text);

    size_t written = fwrite(text, 1, len, fp);

    if (written != len)
    {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return (ssize_t)written;
}

void bss_print_hex_data(const char *name, const uint8_t *buf, size_t len, size_t max_show)
{
    printf("%s (%zu bytes): ", name, len);

    size_t n = (len < max_show) ? len : max_show;

    for (size_t i = 0; i < n; i++)
        printf("%02x", buf[i]);

    if (len > max_show)
        printf("...");

    printf("\n");
}
