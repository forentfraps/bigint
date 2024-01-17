#ifndef _arithmetics_h
#define _arithmetics_h
#include <stdio.h>
extern int _bigadd(unsigned char* dst, unsigned char* op1, unsigned char* op2, int byte_sz);
extern void _aes_prng(unsigned char* state, unsigned char* buffer, size_t sz);
extern void _seed256(unsigned char* state);
extern void _store_le(unsigned char* dst, unsigned char* src, size_t sz);
extern void _load_le(unsigned char* dst, unsigned char* src, size_t sz);
extern void _prime_prng_fix(unsigned char* buf, size_t sz);
extern void _bigmul(unsigned char* dst, unsigned char* op1, unsigned char* op2, size_t sz);
extern int _bigcmp(unsigned char* op1, unsigned char* op2, size_t sz);
extern void _bigneg_c(unsigned char* op1, size_t sz);
extern void _bigmod(unsigned char* op1dst, unsigned char* op2, size_t sz);
extern void _bigdivmod(unsigned char* q, unsigned char* r, unsigned char* op1, unsigned char* op2, size_t sz);
extern void _bigzero(unsigned char * dst, size_t sz);
extern void _bigadd2n(unsigned char* dst, unsigned long long n2, size_t sz);
extern void _bigdivr(unsigned char* dst, size_t sz);
extern void _bigmodr(unsigned char* dst, size_t sz);



int GCDE(unsigned char* a, unsigned char* b, unsigned char* ptr_x, unsigned char* ptr_y, size_t sz);
int ModInverse(unsigned char* a, unsigned char* b, unsigned char* ptr_x, size_t sz);
void print_block(unsigned char* b);
void pbl(unsigned char* b);
void pbl_sz(unsigned char* b, size_t sz);

typedef struct {
    unsigned char* modulus;
    unsigned char* R;
    unsigned char* Rinv;
    unsigned char* Ninv;
    size_t length;
} MNTG;

#define BIG_alloc(name, sz)\
unsigned char name[sz]

#define MNTG_palloc(name, modulus, R, Rinv, Ninv, sz)\
BIG_alloc(modulus, sz);\
BIG_alloc(R, sz);\
BIG_alloc(Rinv, sz);\
BIG_alloc(Ninv, sz);\
MNTG name = {modulus, R, Rinv, Ninv, sz}

int MNTG_setup(MNTG* in);
int MNTG_MUL(MNTG* mntg, unsigned char* dst, unsigned char* op1, unsigned char* op2);
int MNTG_REDC(MNTG* mntg, unsigned char* value);
int MNTG_in(MNTG* mntg, unsigned char* value);


extern void _MNTG_POWMOD(MNTG* mntg, unsigned char* dst, unsigned char* op1, unsigned char* exp);
int MNTG_POWMOD(MNTG* mntg, unsigned char* dst, unsigned char* op1, unsigned char* exp);
#endif