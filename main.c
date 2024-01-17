#include <string.h>
#include "arithmetics.h"

// extern void __bigmul2(unsigned char* op1dst, unsigned long long sz);
// extern void __bigdiv2(unsigned char* op1dst, unsigned long long sz);


#define BigAdd(dst, op1,op2, bit_sz) _bigadd(dst, op1, op2, bit_sz / 8)



int main(){
    size_t sz = 3072>>3;
    size_t sz4 = sz>>1;
    unsigned char seed256[32];
    // unsigned char buf[128];
    BIG_alloc(m1, sz);
    BIG_alloc(m2, sz);
    BIG_alloc(m3, sz);
    MNTG_palloc(mntg,mod, p2, p2inv, negNinv, sz, b1, b2, b3);
    _seed256(seed256);
    _aes_prng(seed256, m1, sz);
    _aes_prng(seed256, m2, sz);
    _aes_prng(seed256, mod, sz);
    // memset(m1, 0, sz>>1);
    // memset(m2, 0, sz>>1);
    // memset(m3, 0, sz);
    // memset(mod, 0, sz>>1);
    memset(m1, 0, sz>>1);
    memset(m2, 0, sz>>1);
    memset(m3, 0, sz);
    memset(mod, 0, sz>>1);
    // m1[sz -1] = 251;
    // m2[sz -1] = 107;
    // mod[sz - 1] = 133;
    // unsigned long long a = 0xc103f70ea9efd2ab;
    // unsigned long long b = 0x7e3ee438fb2c71eb;
    // unsigned long long c = 0x6eccd7dadc525e3f;
    // memcpy(m1 + sz4, &a, 8);
    // memcpy(m2 + sz4, &b, 8);
    // memcpy(mod + sz4, &c, 8);
    // mod[sz -1] |= 1;
    // _store_le(m1,m1, sz);
    // _store_le(m2,m2, sz);
    // _store_le(mod,mod, sz);
    if (MNTG_setup(&mntg)){
        printf("Bad modulo!\n");
        return 1;
    }
    pbl_sz(m1, sz);
    pbl_sz(m2, sz);
    pbl_sz(mod, sz);
    // _bigmul(m3, m1, m2, sz);
    MNTG_in(&mntg, m1);
    // MNTG_in(&mntg, m2);
    printf("montgom vals\n");
    pbl_sz(m1, sz);
    // pbl_sz(m2, sz);
    printf("Results:\n");
    MNTG_POWMOD(&mntg, m3, m1, m2);
    // MNTG_MUL(&mntg, m3, m1, m2);
    MNTG_REDC(&mntg, m3);
    pbl_sz(m3, sz);
    return 0;
}