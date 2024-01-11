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
    gcd = GCDE(b, r, x1, y1, sz);
    if (gcd){
        return 1;
    }
    _bigzero(ptr_y, sz);
    memcpy(ptr_x, y1, sz);
    _bigmul(ptr_y, y1, q, sz);
    _bigneg_c(ptr_y, sz);
    _bigadd(ptr_y, x1, ptr_y, sz);
    return gcd;
}

int ModInverse(unsigned char* a, unsigned char* b, unsigned char* ptr_x, size_t sz){
    int gcd;
    unsigned char ptr_y[sz];
    if (GCDE(a, b, ptr_x, ptr_y, sz) == 0){
        if (ptr_x[0] == 0xff){
            _bigadd(ptr_x, ptr_x, b, sz);
        }
        return 0;
    }
    return 1;
}

/*
typedef struct {
    unsigned char* value;
    unsigned char* modulus;
    unsigned char* mval;
    unsigned char* R;
    unsigned char* Rinv;
    unsigned char* Ninv;
    size_t length;
} MNTG;
*/




int MNTG_in(MNTG* in){
    unsigned char* R = in->R;
    size_t sz = in->length;
    BIG_alloc(rmodn, sz);
    _bigzero(rmodn, sz);
    _bigadd2n(rmodn, sz * 4, sz);
    memcpy(R, rmodn, sz);
    _bigmod(rmodn, in->modulus, sz);
    ModInverse(rmodn, in->modulus, in->Rinv, sz);
    ModInverse(in->modulus, R, in->Ninv, sz);
    _bigneg_c(in->Ninv, sz);
    _bigadd(in->Ninv, in->Ninv, R, sz);
    _bigzero(in->mval, sz);
    _bigmul(in->mval, rmodn, in->value, sz);
    _bigmod(in->mval, in->modulus, sz);
}