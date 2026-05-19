#include <stdio.h>
#include <stdint.h>
#include <sodium.h>
#include "bss_utils.h"

#define SODIUM_PUBLIC_KEY_SIZE    32
#define SODIUM_PRIVATE_KEY_SIZE   64

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <public.key> <private.key>\n", argv[0]);
        return -1;
    }

    const char *pub_path = argv[1];
    const char *priv_path = argv[2];
    int ret;

    uint8_t pub_key[SODIUM_PUBLIC_KEY_SIZE];
    uint8_t priv_key[SODIUM_PRIVATE_KEY_SIZE];

    /* sodium_init */
    ret = sodium_init();
    BSS_CHECK(ret >= 0, "sodium init");

    /* generate keypair */
    ret = crypto_sign_keypair(pub_key, priv_key);
    BSS_CHECK(ret == 0, "create keypair");

    /* write public key */
    ret = bss_write_binary_file(pub_path, (const char *)pub_key, SODIUM_PUBLIC_KEY_SIZE);
    BSS_CHECK(ret > 0, "write public key");

    /* write private key */
    ret = bss_write_binary_file(priv_path, (const char *)priv_key, SODIUM_PRIVATE_KEY_SIZE);
    BSS_CHECK(ret > 0, "write private key");

    printf("Keys generated successfully:\n");
    bss_print_hex_data("    Public Key : ", pub_key, SODIUM_PUBLIC_KEY_SIZE, SODIUM_PUBLIC_KEY_SIZE);
    bss_print_hex_data("    Private Key : ", priv_key, SODIUM_PRIVATE_KEY_SIZE, SODIUM_PRIVATE_KEY_SIZE);

    return 0;
}

