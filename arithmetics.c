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

void pbl_sz(unsigned char* b, size_t sz){
    _load_le(b, b, sz);
    for(int i = 0; i < sz; ++i){
        printf("%02x", b[i]);
        if ((i + 1) % 32 == 0){
            printf("\n");
        }
    }
    printf("\n\n");
    _load_le(b, b, sz);
    return;
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




int MNTG_setup(MNTG* in){
    unsigned char* R = in->R;
    size_t sz = in->length;
    BIG_alloc(rmodn, sz);

    _bigzero(rmodn, sz);
    _bigadd2n(rmodn, sz << 2, sz);
    memcpy(R, rmodn, sz);
    // _bigmod(rmodn, in->modulus, sz);
    if (ModInverse(rmodn, in->modulus, in->Rinv, sz)){
        return 1;
    }
    if (ModInverse(in->modulus, R, in->Ninv, sz)){
        return 1;
    }
    _bigneg_c(in->Ninv, sz);
    _bigadd(in->Ninv, in->Ninv, R, sz);
    return 0;
}

int MNTG_in(MNTG* mntg, unsigned char* value){
    size_t sz = mntg->length;
    BIG_alloc(mval, sz);
    _bigzero(mval, sz);
    _bigmul(mval, mntg->R, value, sz);
    _bigmod(mval, mntg->modulus, sz);
    memcpy(value, mval, sz);
}

int MNTG_REDC(MNTG* mntg, unsigned char* value){
    size_t sz = mntg->length;
    BIG_alloc(m, sz);
    BIG_alloc(mn, sz);
    BIG_alloc(t, sz);
    printf("Value to reduce\n");
    pbl_sz(value, sz);
    memcpy(t, value, sz);
    _bigmodr(t, sz);
    _bigzero(m, sz);
    _bigzero(mn, sz);
    _bigmul(m, t, mntg->Ninv, sz);
    _bigmodr(m, sz);
    printf("m\n");
    pbl_sz(m, sz);
    _bigmul(mn, mntg->modulus, m, sz);
    printf("first multiplication\n");
    pbl_sz(mn, sz);
    _bigadd(value, value, mn, sz);
    printf("addition\n");
    pbl_sz(value, sz);
    _bigdivr(value, sz);
    _bigzero(value, sz >> 1);
    printf("t small\n");
    pbl_sz(value, sz);
    if (_bigcmp(mntg->modulus, value, sz) != 0){
        printf("path is taken\n");
        memcpy(m, mntg->modulus, sz);
        _bigneg_c(m, sz);
        _bigadd(value, value, m, sz);
    }
    return 0;
}

int MNTG_MUL(MNTG* mntg, unsigned char* dst, unsigned char* op1, unsigned char* op2){
    size_t sz = mntg->length;
    BIG_alloc(mul_tmp, sz);
    _bigzero(mul_tmp, sz);
    _bigmul(mul_tmp, op1, op2, sz);
    printf("multiplication\n");
    pbl_sz(mul_tmp, sz);
    memcpy(dst, mul_tmp, sz);
    MNTG_REDC(mntg, dst);
    return 0;
}

int MNTG_pro(MNTG* mntg, unsigned char* dst, unsigned char* op1, unsigned char* op2){
    size_t sz= mntg->length;
    BIG_alloc(t, sz);
    BIG_alloc(m, sz);
}

int MNTG_POWMOD(MNTG* mntg, unsigned char* dst, unsigned char* op1, unsigned char* exp){
    size_t sz = mntg->length;
    BIG_alloc(tmp, sz);
    unsigned long long shl;
    _bigzero(tmp, sz);
    tmp[sz - 1] = 1;
    _store_le(tmp, tmp, sz);
    MNTG_in(mntg, tmp);
    _MNTG_POWMOD(mntg, tmp, op1, exp);
    memcpy(dst, tmp, sz);
    return 0;
}

int POWMOD(unsigned char* dst, unsigned char* op1, unsigned char* exp){

}

