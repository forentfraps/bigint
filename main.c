#include <stdio.h>
#include <string.h>

extern int _bigadd(unsigned char* dst, unsigned char* op1, unsigned char* op2, int byte_sz);
extern void _aes_prng(unsigned char* state, unsigned char* buffer, unsigned long long sz);
extern void _seed256(unsigned char* state);
extern void _store_le(unsigned char* dst, unsigned char* src, unsigned long long sz);
extern void _load_le(unsigned char* dst, unsigned char* src, unsigned long long sz);
extern void _prime_prng_fix(unsigned char* buf, unsigned long long sz);
extern void _bigmul(unsigned char* dst, unsigned char* op1, unsigned char* op2, unsigned long long sz);
extern int _bigcmp(unsigned char* op1, unsigned char* op2, unsigned long long sz);
extern void __bigneg(unsigned char* op1, unsigned long long sz);
extern void __bigpos(unsigned char* op1, unsigned long long sz);
extern void _bigmod(unsigned char* op1dst, unsigned char* op2, unsigned long long sz);
extern void __bigmul2(unsigned char* op1dst, unsigned long long sz);

#define BigAdd(dst, op1,op2, bit_sz) _bigadd(dst, op1, op2, bit_sz / 8)

void print_block(unsigned char* b){
    for (char i =0; i < 32; i++){
        printf("%02x", b[i]);
    }
    printf("\n\n");
    // for(int i = 0; i <)
}

int main(){

    unsigned char seed256[32];
    // unsigned char buf[128];
    _seed256(seed256);
    // printf("Seed:\n");
    // print_block(seed256);
    // print_block(buf);
    // print_block(buf + 32);
    // print_block(buf + 64);
    // print_block(buf + 96);
    unsigned char b1[64];
    unsigned char b2[64];
    unsigned char dst[64];
    _aes_prng(seed256, b1, 64);
    _aes_prng(seed256, b2, 64);
    memset(b1, 0, 32);
    memset(b2, 0, 48);
    // memcpy(b2 + 32, b1+32, 32);
    // b1[63] = 173;
    // b2[63] = 3;

    printf("Block 1\n");
    print_block(b1);
    print_block(b1 + 32);

    printf("Block 2\n");
    print_block(b2);
    print_block(b2 + 32);

    _store_le(b1, b1, 64);
    _store_le(b2, b2, 64);
    // memset(dst, 0, 64);

    _bigmod(b1, b2, 64);
    // __bigmul2(b1, 64);
    _load_le(b1, b1, 64);
    printf("Add is:\n");
    print_block(b1);
    print_block(b1 + 32);
    return 0;
}