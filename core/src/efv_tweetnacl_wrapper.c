#include "tweetnacl.c"

void randombytes(u8 *buf, u64 len)
{
    (void)buf;
    (void)len;
}

int efv_ed25519_tweetnacl_verify(const u8 *ram, u32 ram_len, const u8 s[32])
{
    u8 h[64], t[32];
    gf p[4], q[4];

    if (ram == 0 || s == 0) {
        return -1;
    }

    if (ram_len < 64u) {
        return -1;
    }

    if (unpackneg(q, ram + 32)) {
        return -1;
    }

    crypto_hash(h, ram, ram_len);
    reduce(h);

    scalarmult(p, q, h);
    scalarbase(q, s);
    add(p, q);
    pack(t, p);

    if (crypto_verify_32(ram, t)) {
        return -1;
    }

    return 0;
}