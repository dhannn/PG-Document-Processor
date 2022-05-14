#include "../pgdocs.h"
#include "internals.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NOT_METADATA -1
#define METADATA (!NOT_METADATA)

#define CLEAN_CONTENT_START_SIGNIFIER "*** Start"
#define ANALYZE_CONTENT_START_SIGNIFIER "Content:"

void set_infile(Summary *summary, char *filename);
void set_outfile(Summary *summary, char *filename);
void close_files(Summary *summary);
void set_metadata(MetadataItem items[], int metadataIndex, char *data);
void set_options(Summary *summary, Config config, int rawInput);
int get_metadata_index(MetadataItem metadataItems[], char *str);

void seek_metadata(FILE *file, MetadataItem items[], int buffSize)
{
    int flag = NOT_METADATA;
    char buff[buffSize];

    // increments the pointer until a metadata item is found
    while(flag == NOT_METADATA) {
        fscanf(file, "%s", buff);

        buff[strlen(buff) - 1] = '\0';

        flag = get_metadata_index(items, buff);
    }

    // redirects current pointer right before the first metadata item
    fseek(file, -(strlen(buff)) - 1, SEEK_CUR);
}

char *_get_starting_token(Mode mode)
{
    if(mode == CLEAN)
        return CLEAN_CONTENT_START_SIGNIFIER;
        
    return ANALYZE_CONTENT_START_SIGNIFIER;
}

void read_metadata(FILE *file, MetadataItem items[], Mode mode)
{
    char buff[BUFSIZ];
    int flag;

    char *startingToken = _get_starting_token(mode);
    
    do {
        flag = fscanf(file," %[^:]: s", buff);
        int index = get_metadata_index(items, buff);

        if(index != NOT_METADATA) {
            flag = fscanf(file,"%[^\n]s", buff);
            set_metadata(items, index, buff);
        }
    } while(strcmp(buff, startingToken) && flag != EOF);
}

void read_content(FILE *file, char **inputData, int maxChar)
{
    int flag;               // flag to determine if EOF

    int runningTotal = 0;   // total number of characters
    char *temp = calloc(1, sizeof(char)); // allocation for the content string

    char buff[BUFSIZ];

    do {
        flag = fscanf(file, "%s", buff);
        runningTotal += strlen(buff) + 1; // + 1 for the space
        
        if(maxChar != 0 && runningTotal > maxChar)
            break;

        temp = realloc(temp, runningTotal + 1); // + 1 for null byte

        strcat(buff, " ");
        strcat(temp, buff);
    } while(flag != EOF);

    *inputData = temp;
}

void set_metadata(MetadataItem items[], int metadataIndex, char *data)
{
    char *temp = malloc(strlen(data) + 1);
    strcpy(temp, data);
    items[metadataIndex].data = temp;
}

int get_metadata_index(MetadataItem metadataItems[], char *str)
{
    int i;
    int flag = NOT_METADATA;
    
    for(i = 0; i < MAX_METADATA_ITEMS && flag == -1; i++) {
        if(strcmp(metadataItems[i].name, str) == 0)
            flag = i;
    }
    
    return flag;
}

void deallocate_metadata();
void deallocate_content_string();
void deallocate_tokenized_string();

TokenList *tokenize_string(char *input, bool includeSpace)
{
    TokenList *tokenlist = initialize_tokenlist();
    char *ptrStartChar = input;
    char *curr = input + 1;
    int isPrevAlpha = isalpha(*curr);
    int isCurrAlpha;
    int charCount = 1;

    while(*curr != '\0') {
        // bypasses the current character if excluding spaces
        if(!includeSpace && *curr == ' ') {
            isPrevAlpha = isalpha(*curr);
            curr++;
            continue;
        }

        isCurrAlpha = isalpha(*curr);

        // increments if current & previous char are part of alphabetic string
        if(isPrevAlpha && isCurrAlpha) {
            charCount++;
        } else {
            // allocates the previous alphabetic string and adds it to token
            char *temp = calloc(charCount + 1, sizeof(char));
            strncpy(temp, ptrStartChar, charCount);
            add_token(tokenlist, temp);

            // resets the pointer to the new non-alphabetic char
            ptrStartChar = curr;
            charCount = 1;
        }

        isPrevAlpha = isalpha(*curr);
        curr++;
    }
    
    // allocates the last token that wasn't processed 
    // due to the null byte condition
    char *temp = calloc(charCount + 1, sizeof(char));
    strncpy(temp, ptrStartChar, charCount);
    add_token(tokenlist, temp);

    return tokenlist;
}

void sort_tokens(TokenList *tl)
{
    TokenNode *sortedNode = tl->head;

    while(sortedNode != NULL) {
        TokenNode *max = sortedNode;

        TokenNode *curr = max->next;
        while(curr != NULL) {
            if(curr->frequency > max->frequency) {
                max = curr;
            }

            curr = curr->next;
        }

        swap(sortedNode, max);
        sortedNode = sortedNode->next;
    }
}

void swap(TokenNode *a, TokenNode *b)
{
    if(a == NULL || b == NULL) return;
    
    int tempInt = a->frequency;
    a->frequency = b->frequency;
    b->frequency = tempInt;
    
    char *tempStr = a->tokenString;
    a->tokenString = b->tokenString;
    b->tokenString = tempStr;

    TokenType tempType = a->tokenType;
    a->tokenType = b->tokenType;
    b->tokenType = tempType;
}

void delete_tokens(TokenList *tokenList)
{
    TokenNode *tokenNode = tokenList->head;

    while(tokenNode != NULL) {
        free(tokenNode->tokenString);

        tokenNode = tokenNode->next;
    }
}
