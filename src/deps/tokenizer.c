#include "../pgdocs.h"
#include "internals.h"

#include <stdlib.h>

/* -------------------------------------------------------------------------- */
/*                         PRIVATE FUNCTION PROTOTYPES                        */
/* -------------------------------------------------------------------------- */
/* ------------------ functions only visible to tokenizer.c ----------------- */
void _destroy_tokens(TokenNode *head);
void _recurse_add_token(TokenNode **head, char *token);
void _recurse_remove_token(TokenNode **head, char *token);


/* -------------------------------------------------------------------------- */
/*                              PUBLIC FUNCTIONS                              */
/* -------------------------------------------------------------------------- */
/* ------------------ functions visible to outside modules ------------------ */
TokenList *initialize_tokenlist()
{
    TokenList *tokenList = malloc(sizeof(TokenList));

    tokenList->tokens = NULL;
    tokenList->size = 0;

    return tokenList;
}

void add_token(TokenList *tokenList, char *token)
{
    _recurse_add_token(&tokenList->tokens, token);
    tokenList->size++;
}

void remove_token(TokenList *tokenList, char *token)
{
    _recurse_remove_token(&tokenList->tokens, token);
}

void destroy_tokenList(TokenList *tokenList)
{
    if(tokenList->tokens != NULL)
        _destroy_tokens(tokenList->tokens);
    free(tokenList);
}

/* -------------------------------------------------------------------------- */
/*                              PRIVATE FUNCTIONS                             */
/* -------------------------------------------------------------------------- */
/* ------------------ functions only visible to tokenizer.c ----------------- */
void _recurse_add_token(TokenNode **head, char *token)
{
    if(*head == NULL) {
        *head = malloc(sizeof(TokenNode));
        (*head)->token = token;
        (*head)->next = NULL;
    } else {
        _recurse_add_token(&(*head)->next, token);
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
