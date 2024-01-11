#include "arithmetics.h"
#include <string.h>
void print_block(unsigned char* b){
    for (char i =0; i < 32; i++){
        printf("%02x", b[i]);
    }
    printf("\n\n");
    // for(int i = 0; i <)
}

void pbl(unsigned char* b){
    _load_le(b, b, 64);
    print_block(b);
    print_block(b + 32);
    _load_le(b, b, 64);
}

int GCDE(unsigned char* a, unsigned char* b, unsigned char* ptr_x, unsigned char* ptr_y, unsigned long long sz){
    int gcd;
    unsigned char x1[sz];
    unsigned char y1[sz];
    unsigned char q[sz];
    unsigned char r[sz];
    _bigzero(q, sz);
    // _bigzero(r, sz);

    if (_bigcmp(q, b, sz) == 2){
        q[sz - 8] = 1;

        if (_bigcmp(q, a, sz) != 2){
            return 1;
        }
        _bigzero(ptr_x, sz);
        _bigzero(ptr_y, sz);
        ptr_x[sz - 8] = 1;
        return 0;
    }
    _bigdivmod(q, r, a, b, sz);
    // printf("q\n");
    // pbl(q);
    // printf("r\n");
    // pbl(r);
    gcd = GCDE(b, r, x1, y1, sz);
    if (gcd){
        return 1;
    }
/*
    _bigmul(x1, x1, q, sz);
    _bigneg_c(x1, sz);
    _bigadd(ptr_x, y1, x1, sz);
    memcpy(ptr_y, x1, sz);
*/
    _bigzero(ptr_y, sz);
    memcpy(ptr_x, y1, sz);
    _bigmul(ptr_y, y1, q, sz);
    _bigneg_c(ptr_y, sz);
    _bigadd(ptr_y, x1, ptr_y, sz);
    return gcd;
}