/*******************************************************************************
 * 
 * FILE             utils.c
 * LAST MODIFIED    06-16-2022
 * 
 * DESCRIPTION
 *      This file contains function implementations that do not belong
 *      to other logical units of the source code
 * 
 ******************************************************************************/

#include "../pgdocs.h"
#include "internals.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

TokenList *tokenize_string(char *input, bool includeSpace)
{
    TokenList *tokenlist = initialize_tokenlist();
    char *ptrStartChar = input;
    char *curr = input + 1;
    int isPrevAlpha = isalpha(*curr);
    int isCurrAlpha;
    int charCount = 1;
    int numWords = 0;
    
    while(*curr != '\0') {
        // bypasses the current character if excluding spaces
        if(!includeSpace && isspace(*curr)) {
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

            numWords++;
            update_tokenizing(temp, numWords);
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

TokenList **tokenize_corpus(char **corpusData)
{
    int i;
    int numTokenList = 1;
    TokenList **tokenlists = malloc(sizeof(TokenList*));

    for(i = 0; corpusData[i] != NULL; i++) {
        char *currentCorpusStr = corpusData[i];

        if(i >= numTokenList) {
            numTokenList *= 4;
            tokenlists = realloc(tokenlists, sizeof(TokenList*) * numTokenList);
        }
                        
        tokenlists[i] = tokenize_string(currentCorpusStr, false);
    }

    tokenlists[i] = NULL;
    
    return tokenlists;
}

TokenList *remove_duplicate_tokens(TokenList *tl)
{
    HashTable *ht = create_hash_table();
    
    TokenList *cleanedTl = initialize_tokenlist();
    TokenNode *currentNode = tl->head;

    while(currentNode != NULL) {
        char *currentTokenStr = currentNode->tokenString;
        int len = strlen(currentTokenStr);

        if(*currentTokenStr == ' ') {
            currentNode = currentNode->next;
            continue;
        }

        if(!contains(ht, currentTokenStr)) {
            char *newTokenStr = malloc(len + 1);
            strcpy(newTokenStr, currentTokenStr);
            add_element(ht, newTokenStr);
            add_token(cleanedTl, newTokenStr);
        }

        currentNode = currentNode->next;
    }

    destroy_hash_table(ht);
    return cleanedTl;
}

void sort_tokens_by_freq(TokenList *tl)
{
    TokenNode *sortedNode = tl->head;

    while(sortedNode != NULL) {
        TokenNode *max = sortedNode;

        TokenNode *curr = max->next;
        while(curr != NULL) {
            if(curr->frequency > max->frequency)
                max = curr;

            curr = curr->next;
        }

        swap(sortedNode, max);
        sortedNode = sortedNode->next;
    }
}

void sort_tokens_by_tfidf(TokenList *tl)
{
    TokenNode *sortedNode = tl->head;

    while(sortedNode != NULL) {
        TokenNode *max = sortedNode;

        TokenNode *curr = max->next;
        while(curr != NULL) {
            if(curr->tfidf > max->tfidf)
                max = curr;

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

    float tempTfidf = a->tfidf;
    a->tfidf = b->tfidf;
    b->tfidf = tempTfidf;
}

char *create_string(char *str)
{
	int length = strlen(str) + 1;
	char *temp = calloc(length, sizeof(char));

	strcpy(temp, str);
	return temp;
}

void delete_token_strings(TokenList *tokenList)
{
    if(tokenList == NULL) return;
    TokenNode *tokenNode = tokenList->head;

    while(tokenNode != NULL) {
        free(tokenNode->tokenString);

        tokenNode = tokenNode->next;
    }
}

void fprint_metadata_item(FILE *file, const char *metadata, const char *data)
{
    fprintf(file, "%s: %s\n", metadata, data);
}
