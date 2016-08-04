#ifndef GOST_3411_2012_CALC_H
#define GOST_3411_2012_CALC_H

#include <stdfix.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>

#include "gost_3411_2012_const.h"

#define BLOCK_SIZE 64

typedef uint8_t vect[BLOCK_SIZE];

typedef struct GOSTHashContext
{
    vect buffer;
    vect hash;
    vect h;
    vect N;
    vect Sigma;
    vect v_0;
    vect v_512;
    size_t buf_size;
    int hash_size;
} TGOSTHashContext;

void
GOSTHashInit(TGOSTHashContext *CTX, uint16_t hash_size);

void
GOSTHashUpdate(TGOSTHashContext *CTX, const uint8_t *data, size_t len);

void
GOSTHashFinal(TGOSTHashContext *CTX);

#endif // GOST_3411_2012_CALC_H
