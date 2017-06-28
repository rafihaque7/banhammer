//Some Parts Provided by Darrell Long
# include "hash.h"
# include <string.h>
//# include "aes.c"
# include "ll.c"



static inline int realLength(int l)
{
	return 16 * (l / 16 + (l % 16 ? 1 : 0));
}

uint32_t hash(hashTable *h, const char *key)	//Will return the position that it needs to get stored in
{
	uint32_t output[4] = { 0x0 };
	uint32_t sum       = 0x0;
	int keyL           = strlen(key);
	uint8_t *realKey   = (uint8_t *) calloc(realLength(keyL), sizeof(uint8_t));

	memcpy(realKey, key, keyL);

	for (int i = 0; i < realLength(keyL); i += 16)
	{
		AES128_ECB_encrypt((uint8_t *) h->s, 	        // Salt
                                   (uint8_t *) realKey + i,	// Input
				   (uint8_t *) output);	        // Output
		sum ^= output[0] ^ output[1] ^ output[2] ^ output[3];
	}
	free(realKey);
	return sum%(h->l);
	//return sum%(bloomSize);
}

hashTable *newHT(uint32_t l, uint32_t b[])	//Will set aside memory
{
	hashTable *a = malloc(sizeof(struct hashTable));
	for(int i=0;i<4;i++)
	{
		a->s[i] = b[i];
	}
	a->l = l;

	//a->h = malloc(sizeof(listNode)*l);
	a->h = calloc(l,sizeof(listNode));

	return a;
}

void delHT(hashTable *a)
{
	//  free(a->h);
	//  free(a);

	//Help from TA
	for(uint32_t i=0; i < a->l ;i++)
	{
		delLL(a->h[i]);	//Free up the individual hash positons
	}
	free(a->h);	//Then free up the node
	free(a);



}

listNode *findHT(hashTable *a, const char *word)	//Will give the node of that word, so you can do ->newspeak
{
	uint32_t pos = hash(a,word);
	// return a->h[pos];
	// return findLL(a->h,word);
	// if(moveToFront)
	// {
	// 	return moveToFrontfindLL(&(a->h[pos]),word);
	// }
	// else
	// {
	// 	return findLL(&(a->h[pos]),word);
	// }
	return findLL(&(a->h[pos]),word);
	//return moveToFrontfindLL(&(a->h[pos]),word);
}

void insertHT(hashTable *a, const char *old, const char *new)
{
	uint32_t pos = hash(a,old);
	if(a->h[pos]==NULL)		//If it does not detect a collison
	{
		a->h[pos] = newNode(old,new);
	}
	else
	{
		a->h[pos] = insertLL(&(a->h[pos]),old,new);	//if it detects a collison
	}

}

void printHT(const hashTable *a)
{
	printf("printHT is being used at the moment\n" );
	for(uint32_t i=0; i<a->l;i++)	//This is only for testing purposes
	{
		printLL(a->h[i]);
	}
}

// int main()
// {
// 	uint32_t initH[] = {0xDeadD00d, 0xFadedBee, 0xBadAb0de, 0xC0c0Babe}; // Hash table
// 	hashTable *c = newHT(1,initH);
// 	//insertHT(c,"hey","you");
// 	//insertHT(c,"hello","you");
// 	listNode **n = c->h;
//
//
//
// 	//listNode *temp = insertLL(&(c->h[0]),"The","Boss");
// 	// listNode *temp = insertLL(&(n[0]),"The","Boss");
// 	// listNode *temp2 = insertLL(&(n[0]),"giant","stuff");
// 	//
//
// 	listNode *newff = newNode("The","Boss");
//
// 	listNode *tempForFind = findLL(&(n[0]),"The");
// 	// printf("%s\n",temp->oldspeak);
//
// 	//listNode *aNode = newNode("You suck","You win");
// 	//listNode temp = findLL(aNode,"hello");
// 	//printf("%s",temp->oldspeak);
// 	//printHT(c);
// 	//printLL(temp2);
// 	printf("%s",tempForFind->oldspeak);
//
// 	return 0;
// }

// int main()
// {
// 	uint32_t initH[] = {0xDeadD00d, 0xFadedBee, 0xBadAb0de, 0xC0c0Babe}; // Hash table
// 	hashTable *c = newHT(10,initH);
// 	printf("%s\n",hash() );
// 	return 0;
// }
