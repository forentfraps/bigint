#include <string.h>
#include "arithmetics.h"

// extern void __bigmul2(unsigned char* op1dst, unsigned long long sz);
// extern void __bigdiv2(unsigned char* op1dst, unsigned long long sz);


#define BigAdd(dst, op1,op2, bit_sz) _bigadd(dst, op1, op2, bit_sz / 8)



int main(){

    unsigned char seed256[32];
    // unsigned char buf[128];
    _seed256(seed256);
    unsigned char b1[64];
    unsigned char b2[64];
    unsigned char n[64];
    unsigned char dst[64];
    _aes_prng(seed256, b1, 64);
    _aes_prng(seed256, b2, 64);
    _aes_prng(seed256, n, 64);
    memset(b1, 0, 32);
    memset(b2, 0, 32);
    memset(n, 0, 64);
    memset(dst, 0, 64);
    // memcpy(b2 + 32, b1+32, 32);
    // b2[63] = 6;
    // b1[63] = 2;
    // b2[63] = 17;
    printf("Block 1 %p\n", b1);
    print_block(b1);
    print_block(b1 + 32);

    printf("Block 2\n");
    print_block(b2);
    print_block(b2 + 32);

    _store_le(b1, b1, 64);
    _store_le(b2, b2, 64);
    // memset(dst, 0, 64);
    // __bigdiv2(b1, 64);
    // _bigmod(b1, b2, 64);
    // _bigdivmod(n, dst, b2, b1, 64);
    int gcd = GCDE(b1, b2, n, dst, 64);
    if (gcd){
        printf("bad numbers!\n");
    }
    // _bigmul(dst, b1, b2, 64);
    printf("q is:\n");
    pbl(n);
    printf("r is:\n");
    pbl(dst);
    printf("Finish!\n");
    return 0;
}