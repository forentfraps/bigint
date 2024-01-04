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
    unsigned char b1[32];
    unsigned char b2[32];
    unsigned char dst[32];
    // _aes_prng(seed256, b1, 32);
    // _aes_prng(seed256, b2, 32);
    memset(b1, 0, 32);
    memset(b2, 0, 32);
    // b1[31] = 11;
    // b2[31] = 10;

    printf("Blocks before prng fix conversion\n");
    print_block(b1);
    print_block(b2);
    // _prime_prng_fix(b1, 32);
    // _prime_prng_fix(b2, 32);
    // printf("Blocks after prng fix conversion\n");
    // print_block(b1);
    // print_block(b2);
    _store_le(b1, b1, 32);
    _store_le(b2, b2, 32);
    switch (_bigcmp(b1, b2, 32)){
        case 0: printf("First operand is bigger\n");break;
        case 1: printf("Second operand is bigger\n");break;
        case 2: printf("Equal\n");break;
    }
    memset(dst, 0, 32);
    // _bigmul(dst, b1, b2, 64);
    // _load_le(dst, dst, 64);
    // printf("Mul is:\n");
    // print_block(dst);print_block(dst + 32);
    return 0;
}