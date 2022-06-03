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

TokenList *remove_duplicate_tokens(TokenList *tl)
{
    HashTable *ht = create_hash_table();
    
    TokenList *cleanedTl = initialize_tokenlist();
    TokenNode *currentNode = tl->head;

    while(currentNode != NULL) {
        char *currentTokenStr = currentNode->tokenString;

        if(*currentTokenStr == ' ') {
            currentNode = currentNode->next;
            continue;
        }

        if(!contains(ht, currentTokenStr)) {
            add_element(ht, currentTokenStr);
            add_token(cleanedTl, currentTokenStr);
        }

        currentNode = currentNode->next;
    }

    destroy_hash_table(ht);

    return cleanedTl;
}

void sort_tokens(TokenList *tl)
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

void delete_token_strings(TokenList *tokenList)
{
    TokenNode *tokenNode = tokenList->head;

    while(tokenNode != NULL) {
        free(tokenNode->tokenString);

        tokenNode = tokenNode->next;
    }
}
