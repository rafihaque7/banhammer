////Hash function is Provided by Darrell Long, I modified some parts
# include "bf.h"
# include <stdint.h>
# include <stdlib.h>
# include <stdio.h>

# include <string.h>
# include "aes.c"


static int realLengthBF(int l)	//This is in the hash function
{
	return 16 * (l / 16 + (l % 16 ? 1 : 0));
}

uint32_t hashBF(bloomF *h, char *key)
{

    uint32_t output[4] = { 0x0 };
    	uint32_t sum       = 0x0;
    	int keyL           = strlen(key);
    	uint8_t *realKey   = (uint8_t *) calloc(realLengthBF(keyL), sizeof(uint8_t
    ));

    	memcpy(realKey, key, keyL);

    	for (int i = 0; i < realLengthBF(keyL); i += 16)
    	{
    		AES128_ECB_encrypt((uint8_t *) h->s, 	        // Salt
                                       (uint8_t *) realKey + i,	// Input
    				   (uint8_t *) output);	        // Output
    		sum ^= output[0] ^ output[1] ^ output[2] ^ output[3];
    	}
    	free(realKey);
    	return sum % (h->l);	//Difference between hash and hashBF is that hashBF modulos the length of the bloom filter.

}
