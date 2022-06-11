/*******************************************************************************
 * 
 * FILE             token_list.c
 * LAST MODIFIED    04-19-2022
 * 
 * DESCRIPTION
 *      This file contains function implementations that creates, modifies
 *      and handles linked lists containing string tokens.
 * 
 ******************************************************************************/

#include "../pgdocs.h"
#include "internals.h"

#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* -------------------------------------------------------------------------- */
/*                         PRIVATE FUNCTION PROTOTYPES                        */
/* -------------------------------------------------------------------------- */
/* ------------------ functions only visible to tokenizer.c ----------------- */
TokenType __get_token_type(char token);


/* -------------------------------------------------------------------------- */
/*                              PUBLIC FUNCTIONS                              */
/* -------------------------------------------------------------------------- */
/* ------------------ functions visible to outside modules ------------------ */
TokenList *initialize_tokenlist()
{
    TokenList *tokenList = malloc(sizeof(TokenList));

    tokenList->head = NULL;
    tokenList->iterator = tokenList->head;
    tokenList->size = 0;

    return tokenList;
}

void add_token(TokenList *tokenList, char *token)
{
    TokenType type = __get_token_type(*token);
    
    TokenNode *temp = malloc(sizeof(TokenNode));
    temp->tokenString = token;  
    temp->frequency = 1;
    temp->tokenType = type;
    temp->next = NULL;

    TokenNode **ptrToNode = &tokenList->head;

    while(*ptrToNode != NULL)
        ptrToNode = &(*ptrToNode)->next;
    
    *ptrToNode = temp;
    tokenList->size++;

    if(tokenList->iterator == NULL)
        tokenList->iterator = tokenList->head;
}

void remove_token(TokenList *tokenList, char *token)
{
    if(tokenList->head == NULL) return;

    TokenNode *previous = tokenList->head;
    TokenNode *current = previous->next;
    TokenNode *next = current->next;

    while(current != NULL) {
        if(strcmp(previous->tokenString, token) == 0) {
            tokenList->head = current;
            free(previous);
            break;
        }

        if(strcmp(current->tokenString, token) == 0) {
            previous->next = next;
            free(current);
            break;
        }

        previous = previous->next;
        current = current->next;

        if(next != NULL)
            next = next->next;
    }
}
bool is_token_found(TokenList *tokenList, char *token)
{
    bool isFound = false;
    TokenNode *tokenNode = tokenList->head;

    while(tokenNode != NULL && !isFound) {
        if(strcmp(tokenNode->tokenString, token) == 0)
            isFound = true;
        tokenNode = tokenNode->next;
    }

    return isFound;
}

void print_tokens(TokenList *tokenList)
{
    TokenNode *tokenNode = tokenList->head;

    while(tokenNode != NULL) {
        printf("%s (%d)\n", tokenNode->tokenString, tokenNode->frequency);
        tokenNode = tokenNode->next;
    }
}

void increment_token_frequency(TokenList *tokenList, char *token)
{
    TokenNode *tokenNode = tokenList->head;

    while(tokenNode != NULL) {
        if(strcmp(tokenNode->tokenString, token) == 0)
            tokenNode->frequency++;
        
        tokenNode = tokenNode->next;
    }
}

TokenNode *next_token(TokenList *tokenList)
{
    TokenNode *current = tokenList->iterator;

    if(current == NULL)
        tokenList->iterator = tokenList->head;
    else
        tokenList->iterator = tokenList->iterator->next;

    return current;
}


void destroy_tokenList(TokenList *tokenList)
{
    if(tokenList == NULL) return;

    if(tokenList->head != NULL) {
        TokenNode *prev = tokenList->head;
        TokenNode *curr = prev->next;

        while(curr != NULL) {
            curr = prev->next;
            free(prev);
            prev = curr;
        }
        
        free(prev);
    }

    free(tokenList);
}

/* -------------------------------------------------------------------------- */
/*                              PRIVATE FUNCTIONS                             */
/* -------------------------------------------------------------------------- */
/* ------------------ functions only visible to tokenizer.c ----------------- */

TokenType __get_token_type(char token)
{
    if(isalpha(token) != 0)
        return ALPHA;

    if(isalnum(token) != 0)
        return NUMERIC;

    if(isspace(token) != 0)
        return WHITESPACE;
    
    return SPECIAL;
}
