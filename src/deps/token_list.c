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
void _destroy_tokens(TokenNode *head);
void _recurse_add_token(TokenNode **head, char *token, TokenType type);
void _recurse_remove_token(TokenNode **head, char *token);


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

TokenNode *next_token(TokenList *tokenList)
{
    TokenNode *current = tokenList->iterator;

    if(current == NULL)
        tokenList->iterator = tokenList->head;
    else
        tokenList->iterator = tokenList->iterator->next;

    return current;
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

void increment_token_frequency(TokenList *tokenList, char *token)
{
    TokenNode *tokenNode = tokenList->head;

    while(tokenNode != NULL) {
        if(strcmp(tokenNode->tokenString, token) == 0)
            tokenNode->frequency++;
        
        tokenNode = tokenNode->next;
    }
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

void destroy_tokenList(TokenList *tokenList)
{
<<<<<<< HEAD
<<<<<<< HEAD
    if(tokenList == NULL || tokenList->head == NULL) return;

    TokenNode *prev = tokenList->head;
    TokenNode *curr = prev->next;

    while(curr != NULL) {
        curr = prev->next;
        free(prev);
        prev = curr;
=======
=======
>>>>>>> 91690d72917353b5e900b037eee42c2f3056a193
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
<<<<<<< HEAD
>>>>>>> 91690d72917353b5e900b037eee42c2f3056a193
=======
>>>>>>> 91690d72917353b5e900b037eee42c2f3056a193
    }

    free(tokenList);
}

void print_tokens(TokenList *tokenList)
{
    TokenNode *tokenNode = tokenList->head;

    while(tokenNode != NULL) {
        printf("%s (%d)\n", tokenNode->tokenString, tokenNode->frequency);
        tokenNode = tokenNode->next;
    }
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

void _recurse_add_token(TokenNode **head, char *token, TokenType type)
{
    if(*head == NULL) {
        *head = malloc(sizeof(TokenNode));
        (*head)->tokenString = token;
        (*head)->frequency = 1;
        (*head)->tokenType = type;
        (*head)->next = NULL;
    } else {
        _recurse_add_token(&(*head)->next, token, type);
    }
}

void _recurse_remove_token(TokenNode **head, char *token)
{
    if(*head == NULL) return; // guard clause

    // if the token is in the head node
    if((*head)->tokenString == token) {
        TokenNode *oldHead = *head;
        TokenNode *newHead = (*head)->next;

        *head = newHead;

        oldHead->next = NULL;
        free(oldHead);
        return;
    }

    // if the token is either in the middle or the end of list
    TokenNode *previous = *head;
    TokenNode *current = (*head)->next;
    TokenNode *next = current->next;

    if(current->tokenString == token) {
        previous->next = next;
        current->next = NULL;
        free(current);
    } else if(next != NULL) {
        _recurse_remove_token(&(*head)->next, token);
    }
}

void _destroy_tokens(TokenNode *head)
{
    if(head->next != NULL) {
        _destroy_tokens(head->next);
    }
    
    free(head);
}
