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
TokenType _get_token_type(char token);
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
    tokenList->size = 0;

    return tokenList;
}

void add_token(TokenList *tokenList, char *token)
{
    TokenType type = _get_token_type(*token);
    
    _recurse_add_token(&tokenList->head, token, type);
    tokenList->size++;
}

bool is_token_found(TokenList *tokenList, char *token)
{
    bool isFound = false;
    TokenNode *tokenNode = tokenList->head;

    while(tokenNode != NULL && !isFound) {
        if(strcmp(tokenNode->token, token) == 0)
            isFound = true;
        tokenNode = tokenNode->next;
    }

    return isFound;
}

void increment_token_frequency(TokenList *tokenList, char *token)
{
    TokenNode *tokenNode = tokenList->head;

    while(tokenNode != NULL) {
        if(strcmp(tokenNode->token, token) == 0)
            tokenNode->frequency++;
        
        tokenNode = tokenNode->next;
    }
}

void remove_token(TokenList *tokenList, char *token)
{
    _recurse_remove_token(&tokenList->head, token);
}

void destroy_tokenList(TokenList *tokenList)
{
    if(tokenList->head != NULL)
        _destroy_tokens(tokenList->head);
    free(tokenList);
}

void print_tokens(TokenList *tokenList)
{
    TokenNode *tokenNode = tokenList->head;

    while(tokenNode != NULL) {
        printf("%s (%d)\n", tokenNode->token, tokenNode->frequency);
        tokenNode = tokenNode->next;
    }
}

/* -------------------------------------------------------------------------- */
/*                              PRIVATE FUNCTIONS                             */
/* -------------------------------------------------------------------------- */
/* ------------------ functions only visible to tokenizer.c ----------------- */

TokenType _get_token_type(char token)
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
        (*head)->token = token;
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
    if((*head)->token == token) {
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

    if(current->token == token) {
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
