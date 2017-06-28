# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <getopt.h>
# include <string.h>
# include <errno.h>
# include "bf.c"
# include "hash.c"
# include <ctype.h>

void viewJoycampLetter()    //Will print out if he/she will need to go to joycamp
{
    printf("Dear Comrade,\n\nYou have chosen to use degenerate words that may cause hurt\nfeelings or cause your comrades to think unpleasant thoughts.\nThis is doubleplus bad. To correct your wrongthink and\nsave community consensus we will be sending you to joycamp\nadministered by Miniluv.\n\n");
}

void viewFeelingsLetter()   //Will only print out if he/she is not going to joycamp, but need to chnage words
{
    printf("Dear Comrade,\n\nSubmitting your text helps to preserve feelings and prevent\nbadthink. Some of the words that you used are not goodspeak.\nThe list shows how to turn the oldspeak words into newspeak.\n\n");
}

void printBadWords(char **wordsArray,int counter)   //Prints it out at the end
{
    printf("Your errors:\n\n");
    for(int i=counter-1;i>=0;i--)   //It prints out backward to make it look like Darrell's output
    {
        printf("%s\n",wordsArray[i]);
    }
    printf("\n");
}

void printTranslateWords(char ***wordsArray, int counter)   //Prints out the Translations at the end
{
    printf("Think on these words during your vacation!\n\n");
    for(int i=counter-1; i>=0; i--)      //It prints out backward to make it look like Darrell's output
    {
        printf("%s -> %s\n",wordsArray[i][0],wordsArray[i][1]);
    }
}

void viewStatistics(uint32_t totalDictcount,uint32_t otalTranslatecount,uint32_t statsText,float statsDens1,float statsDens2)   //-s will print this function and nothing else
{
    printf("Seeks %u, Average %f, Dictionary %u, Translations %u, Text %u, Densities: %f, %f",funcSeeks(),funcAverage(),totalDictcount,otalTranslatecount,statsText,statsDens1,statsDens2);
    printf("\n" );
}

int main(int argc, char **argv) //argv is how you get the argument
{
    int option; // for getopt
    uint32_t hashSize = 10000;  //These are all the default values
    uint32_t bloomSize = 1048576;
    bool moveToFront = false;
    bool showStats = false;
    opterr = 0;

    while ((option = getopt(argc, argv, "smbh:f:")) != -1)
    {
        switch(option)
        {
            case 's':   //Will suppress the letter from the censor, just print the statistics
            {
                showStats = true;
                break;
            }
            case 'h':   //Size specifies that the hash table will have
            {
                hashSize=atoi(optarg);
                break;
            }
            case 'f':   //Bloom filter size entries
            {
                bloomSize=atoi(optarg);
                break;
            }
            case 'm':   //Use the move to front rule
            {
                moveToFront = true;
                printf("%d\n",moveToFront );
                break;
            }
            case 'b':   //will not use move-to-front rule
            {
                moveToFront = false;
                break;
            }
        }
    }

    /*
    The Salts for bloomfilters and hash table.
    */
    uint32_t initA[] = {0xDeadD00d, 0xFadedBee, 0xBadAb0de, 0xC0c0aB0a}; // First Bloom filter
    uint32_t initB[] = {0xDeadBeef, 0xFadedB0a, 0xCafeD00d, 0xC0c0aB0a}; // Second Bloom filter
    uint32_t initH[] = {0xDeadD00d, 0xFadedBee, 0xBadAb0de, 0xC0c0Babe}; // Hash table
    bloomF *a = newBF(bloomSize,initA);
    bloomF *b = newBF(bloomSize, initB);
    hashTable *c = newHT(hashSize,initH);
    //Setting Dictionary for badspeak.txt
    char streamWord[100];
    uint32_t numDict = 0;
    FILE *badFile = fopen("badspeak.txt","r");
    //getTokens(a,b,c); // Enter the main loop of the program
    while(!feof(badFile))
    {
        fscanf(badFile,"%s\n",streamWord);
        setBF(a,streamWord);
        setBF(b,streamWord);
        insertHT(c,streamWord,"NULL");
        numDict++;
    }
    fclose(badFile);

    //Setting Dictionary newspeak.txt (Code from Piazza)
    char wordOldspeak[100];
    char wordNewspeak[100];
    uint32_t numTranslates = 0; //This is for statistics, the number of words that got translated
    FILE *nsFile = fopen("newspeak.txt","r");
    while(!feof(nsFile))
    {
        fscanf(nsFile,"%s %s\n",wordOldspeak,wordNewspeak);
        setBF(a,wordOldspeak);
        setBF(b,wordOldspeak);
        insertHT(c,wordOldspeak,wordNewspeak);  //Instead of None, it will be the translation
        numTranslates++;
    }
    fclose(nsFile);
    //For inputting text file
    //For setting statistics for the densities

    float statsDens1 = (float)countBF(a)/(float)lenBF(a);   //For the first density
    float statsDens2 = (float)countBF(b)/(float)lenBF(b);   //For the second density

    int enableJoycamp = 0;  //By default, the user is not going to joy camp, this is basically a bool
    int enableFeelings = 0; //It is basically a bool

    char **storeBadwords;   //A char array to store all the bad words in the input
    int counterBad = 0;
    storeBadwords = malloc(sizeof(char*)*1);

    char ***storeTranslatewords;    //A 2d array to store all the translation words in the input
    int counterTranslate = 0;
    storeTranslatewords = malloc(sizeof(char**)*1);

    uint32_t statsText = 0;

    //The lex analyzer starts here
    int yylex(void);
    extern char* yytext;    //Since its in lex.yy.c
    extern FILE* yyin;
    int tokenID =0;
    yyin = stdin;

    while(tokenID != -1)    //-1 means it is end of the file
    {
        tokenID = yylex();  //will give the tokenID

        //Converts everything to lowercase
        for(int i = 0; yytext[i]; i++)
        {
            yytext[i] = tolower(yytext[i]); //Converts each character
        }
        statsText++;
        if(memBF(a,yytext)==1 && memBF(b,yytext)==1)    //If both bloom filters have a value of 1, then it will move to check hashtable
        {
            listNode *tempnode = findHT(c,yytext);

            if(tempnode != NIL && strcmp(tempnode->oldspeak,yytext)==0) //This means its a badspeak word
            {
                if(strcmp(tempnode->newspeak,"NULL")==0)
                {
                    if(!enableJoycamp) enableJoycamp=1; //If it detects a badword, then it will permenantly set enableJoycamp to 1
                    int assign = 1; //So it makes sure that it does not store the same word twice
                    for(int i=0; i<counterBad; i++)
                    {
                        if(strcmp(yytext,storeBadwords[i])==0) assign=0; //Checks if the word is already stores in storeBadwords
                    }
                    if(assign==1)
                    {
                        storeBadwords = realloc(storeBadwords,sizeof(char*)*(counterBad+1));
                        storeBadwords[counterBad] = tempnode->oldspeak;
                        counterBad++;
                    }
                }
                else
                {
                    if(!enableFeelings) enableFeelings=1;   //If there is just one word that needs to get translated, it will enagleFeelings letter
                    //printf("Translate this %s to %s\n",store,tempnode->newspeak );
                    int assign = 1;
                    for(int i=0; i<counterTranslate;i++)
                    {
                        if(strcmp(yytext,storeTranslatewords[i][0])==0 && strcmp(tempnode->newspeak,storeTranslatewords[i][1])==0)  assign=0;   //It checks to make sure that it is not already in the list
                    }
                    if(assign==1)   //If the word is not already in the storeBadwords list.
                    {
                        storeTranslatewords = realloc(storeTranslatewords,sizeof(char**)*(counterTranslate+1)); //Make 2d array for oldspeak and newspeak
                        storeTranslatewords[counterTranslate] = malloc(sizeof(char*)*2);
                        storeTranslatewords[counterTranslate][0] = tempnode->oldspeak;  //0 is referring to oldspeak
                        storeTranslatewords[counterTranslate][1] = tempnode->newspeak;  //1 is referring to newspeak. Since there is just 2
                        counterTranslate++;
                    }
                }
            }
        }
    }

    if(!showStats)  //If -s is disabled only
    {
        if(enableJoycamp==1) viewJoycampLetter();   //Will show letter
        if(!enableJoycamp&&enableFeelings) viewFeelingsLetter();
        printBadWords(storeBadwords,counterBad);
        printTranslateWords(storeTranslatewords,counterTranslate);
        //printf("%s\n",storeBadwords[1]);
    }
    if(showStats) viewStatistics(numDict,numTranslates,statsText,statsDens1,statsDens2);    //if -p then it will only print statstics

    delBF(a);   //Free memory of bloomF A
    delBF(b);   //Free memory of bloomF B
    delHT(c);   //Free memory of hashTable, which is c
    free(storeBadwords);
    for(int i=0; i<counterTranslate;i++)    //Since store translate is a 2d array
    {
        free(storeTranslatewords[i]);   //First free it individually
    }
    free(storeTranslatewords);
    //printf("%d",hash(c,"you"));
    return 0;
}
