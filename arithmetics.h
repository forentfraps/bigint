#ifndef _arithmetics_h
#define _arithmetics_h
#include <stdio.h>
extern int _bigadd(unsigned char* dst, unsigned char* op1, unsigned char* op2, int byte_sz);
extern void _aes_prng(unsigned char* state, unsigned char* buffer, unsigned long long sz);
extern void _seed256(unsigned char* state);
extern void _store_le(unsigned char* dst, unsigned char* src, unsigned long long sz);
extern void _load_le(unsigned char* dst, unsigned char* src, unsigned long long sz);
extern void _prime_prng_fix(unsigned char* buf, unsigned long long sz);
extern void _bigmul(unsigned char* dst, unsigned char* op1, unsigned char* op2, unsigned long long sz);
extern int _bigcmp(unsigned char* op1, unsigned char* op2, unsigned long long sz);
extern void _bigneg_c(unsigned char* op1, unsigned long long sz);
extern void _bigmod(unsigned char* op1dst, unsigned char* op2, unsigned long long sz);
extern void _bigdivmod(unsigned char* q, unsigned char* r, unsigned char* op1, unsigned char* op2, unsigned long long sz);
extern void _bigzero(unsigned char * dst, unsigned long long sz);


int GCDE(unsigned char* a, unsigned char* b, unsigned char* ptr_x, unsigned char* ptr_y, unsigned long long sz);
void print_block(unsigned char* b);
void pbl(unsigned char* b);
#endif