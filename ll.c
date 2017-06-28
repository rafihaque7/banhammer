//Provided by Darrell Long I modified some parts
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include "ll.h"
//# include <stdint.h>
uint32_t countSeeks = 0;    //For statistics
uint32_t counterSeeks = 0;  //For average

//Taken from stackoverflow, since it is not a c99 standard
char *strdup (const char *s) {
    char *d = malloc (strlen (s) + 1);   // Space for length plus nul
    if (d == NULL) return NULL;          // No memory
    strcpy (d,s);                        // Copy the characters
    return d;                            // Return the new string
}

listNode *newNode(const char *old,const char *new)  //Will create it and return the node location
{
        listNode *t = malloc(sizeof(listNode)); //Allocates the memory
        t->oldspeak = strdup(old);  //strdup is not included in c99 standard
        t->newspeak = strdup(new);
        t->next = NIL;
        return t;
}

void delNode(listNode *n) { free(n->oldspeak); free(n->newspeak); free(n); return; }    //Free individually

void delLL(listNode *n) //Will delete individually
{
        while (n != NIL)
        {
                listNode *t = n;    //Pointing it to n address
                n = n-> next;
                delNode(t);
        }
        return;
}

listNode *insertLL(listNode **n, const char *old, const char *new)  //Will put it in the front and return the position
{
        listNode *t = newNode(old,new);
        t->next = *n;   //Dereferencing **n once as if it was listNode *n
        return t;
}


listNode *moveToFrontfindLL(listNode **n, const char *key)  //Will only get activated if -m flag is there
{
        listNode *tempNode; //This will be returned later
        for (listNode *t = *n; t != NIL; t = t->next)   //Will find the word
        {
            countSeeks++;

                if (t->next != NIL && strcmp(t->next->oldspeak, key) == 0) //If the next word is what we're looking for
                {
                        //listNode *tempNode = insertLL(n,(t)->next->oldspeak,(t)->next->newspeak);
                        tempNode = newNode(t->next->oldspeak,t->next->newspeak);
                        tempNode->next =*n;
                        t->next = t->next->next; //The key node
                        return tempNode;   //Return the location
                        //*n=tempNode;
                }
        }
        return NIL;
}

listNode *findLL(listNode **n, const char *key) //Without move to front rule
{
    countSeeks++;
    counterSeeks++;
        for (listNode *t = *n; t != NIL; t = t->next)   //Will find the word
        {
            countSeeks++;
                if (strcmp(t->oldspeak, key) == 0)
                {
                        return t;   //Return the location
                }
        }
        return NIL;
}
void printLL(listNode *n)   //Prints out the whole linked list
{
        for (listNode *t = n; t != NIL; t = t->next)
        {
                printf("%s ", t->oldspeak);
        }
        printf("\n");
        return;
}

uint32_t funcSeeks()    //For seeks count -s
{
    return countSeeks;
}

float funcAverage()     //-s
{
    return (float)countSeeks/(float)counterSeeks;
}
