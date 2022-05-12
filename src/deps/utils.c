#include "../pgdocs.h"
#include "internals.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

TokenList *tokenize(char *input, bool includeSpace)
{
    TokenList *tokenlist = initialize_tokenlist();
    char *ptrStartChar = input;
    char *curr = input + 1;
    int isPrevAlpha = isalpha(*curr);
    int isCurrAlpha;
    int charCount = 1;

    while(*curr != '\0') {
        if(!includeSpace && *curr == ' ') {
            isPrevAlpha = isalpha(*curr);
            curr++;
            continue;
        }

        isCurrAlpha = isalpha(*curr);

        if(isPrevAlpha && isCurrAlpha) {
            charCount++;
        } else {
            char *temp = calloc(charCount + 1, sizeof(char));
            strncpy(temp, ptrStartChar, charCount);
            add_token(tokenlist, temp);

            ptrStartChar = curr;
            charCount = 1;
        }

        isPrevAlpha = isalpha(*curr);
        curr++;
    }
    
    char *temp = calloc(charCount + 1, sizeof(char));
    strncpy(temp, ptrStartChar, charCount);
    add_token(tokenlist, temp);

    return tokenlist;
}

void sort_tokens(TokenList *tl)
{
    TokenNode *sortedNode = tl->tokens;

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
    
    char *tempStr = a->token;
    a->token = b->token;
    b->token = tempStr;

    TokenType tempType = a->tokenType;
    a->tokenType = b->tokenType;
    b->tokenType = tempType;
}

void delete_tokens(TokenList *tokenList)
{
    TokenNode *tokenNode = tokenList->tokens;

    while(tokenNode != NULL) {
        free(tokenNode->token);

        tokenNode = tokenNode->next;
    }
}
