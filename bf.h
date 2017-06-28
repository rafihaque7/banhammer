//Some parts Provided by Darrell Long
# ifndef NIL
# define NIL (void *) 0
# endif
# ifndef _BF_H
# define _BF_H
# include <stdint.h>
# include <stdlib.h>
# include <stdio.h>

# include <string.h>
# include "aes.h"



typedef struct bloomF { //Can refer to this struct by simply bloomF
    uint8_t *v; //Vector
    uint32_t l; //Length
    uint32_t s[4]; //Salt
}   bloomF;


// Each function has its own hash function, determined by the salt.
static int realLengthBF(int l);
uint32_t hashBF(bloomF *h, char *key);

// Create a new Bloom Filter of a given length and hash function.


static inline bloomF *newBF(uint32_t l, uint32_t b[])   //Creates a new Bloomfilter and sets aside memory
{
    bloomF *a = malloc(sizeof(struct bloomF));  //Struct of that

    for (int i=0; i<4;i++)
    {
        a->s[i] = b[i]; //Will save the salt in ->s
    }
    a->l = l;   //Save the length

    //this code is from Darrell
    a->v = (uint8_t *) calloc(l / 8 + 1, sizeof(uint8_t));  //Will set aside memory and set to 0
    return a;
}

static inline void delBF(bloomF *v)
{
    //Code from Darrell
    free(v->v); //First free the container
    free(v);    //This is the bucket
    return;
}

// Return the value of position k in the Bloom Filter

static inline uint32_t valBF(bloomF *x, uint32_t k) //The value is either 0 or 1
{
    //Code from Darrell
        return (x->v[k >> 3] & (01 << (k & 07))) >> (k & 07);


}

static inline uint32_t lenBF(bloomF *x) { return x->l;} //Simply the struct

//Count bits in the Bloom Filter

static inline uint32_t countBF(bloomF *b)   //Number of bits set to 1 in the bloomfilter
{
    uint32_t count = 0;
    for (uint32_t i = 0; i<lenBF(b);i++)
    {
        if(valBF(b,i)) count++;
    }
    return count;
}

static inline void setBF(bloomF *A, char * key) //Will change the bit from 0 to 1
{
    uint32_t k = hashBF(A,key);
    //Darrell's code
    A->v[k >> 3] |= (01 << (k & 07));

}

//  Clear an entry in the Bloom filter
static inline void clrBF(bloomF *A, char *key)
{
    uint32_t k = hashBF(A,key);
    A->v[k >> 3] &= ~(01 << (k & 07));
}

//Check membership in the Bloom Filter
static inline uint32_t memBF(bloomF *A, char *key)
{
    uint32_t k = hashBF(A,key);

    return (A->v[k >> 3] & (01 << (k & 07))) >> (k & 07);

}

static inline void printBF(bloomF *x)   //For testing purposes, to see if it actually did it
{
    for (uint32_t i = 0; i<x->l;i++)
    {
        printf("%u ",valBF(x,i));
    }
    printf("\n");
}

# endif
