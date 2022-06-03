#ifndef INTERNALS_H
#define INTERNALS_H
#include <stdbool.h>

typedef enum {
    ALPHA,          // alphabetic strings (e.g. "Hello", "wor", "d")
    NUMERIC,        // numeric characters (e.g. "1")
    WHITESPACE,     // whitespace characters (e.g. "\n", " ")
    SPECIAL         // characters not fitting to the previous ones (e.g. "!", ","),
} TokenType;

typedef struct tk{
    char *tokenString;
    int frequency;
    TokenType tokenType;
    struct tk *next;        // pointer for the next node
} TokenNode;

typedef struct {
    TokenNode *head;
    TokenNode *iterator;
    int size;
} TokenList;

typedef struct _hashTable {
    TokenList *tokenList;
    int size;
} HashTable;


/* -------------------------------------------------------------------------- */
/*                       FUNCTIONS FOR TOKEN LINKED LIST                      */
/* -------------------------------------------------------------------------- */

/**
 * initialize_tokenlist()
 * Returns a pointer to an initialized token list
 * 
 * @pre
 * @return      TokenList*              Pointer to a dynamically-allocated list      
 */
TokenList *initialize_tokenlist();

/**
 * add_token()
 * Adds a token to the linked list of tokens
 * 
 * @pre         the linked list is null-terminated
 * @param       TokenList*              the pointer to the list to be added
 * @param       char*                   the token string to be added
 */
void add_token(TokenList *tl, char *token);

/**
 * remove_token()
 * Removes the specified token from the list
 * 
 * @pre         the linked list is null-terminated
 * @param       TokenList*              the pointer to the list to be removed
 * @param       char*                   the token string to be removed
 */
void remove_token(TokenList *tl, char *token);

/**
 * next_token()
 * Iterates through the token list
 * 
 * @pre         the linked list is null-terminated
 * @param       TokenList*              the pointer to the list to be iterated
 */
TokenNode *next_token(TokenList *tokenList);

/**
 * get_token_string()
 * Returns the string of a specified node
 * 
 * @pre         the node is not null
 * @param       TokenNode*              the pointer to the specified node
 * @return      char*                   the token string in the node
 */
char *get_token_string(TokenNode *tokenNode);

/**
 * increment_token_frequency()
 * Increases the frequency of a token.
 * 
 * @pre         tokenList is null terminated
 * @pre         token must exist in the list
 * @pre         token must also be unique (i.e. duplicates are removed)
 * @param       TokenList*              the pointer to the token list
 * @param       char*                   the token string to increment
 */
void increment_token_frequency(TokenList *tokenList, char *token);

/**
 * is_token_found()
 * Checks if the token is in the linked list
 * 
 * @pre         tokenList is null terminated
 * @param       TokenList*              the pointer to the token list
 * @param       char*                   the token string to search
 * @return      bool                    true if found; false if not
 */
bool is_token_found(TokenList *tokenList, char *token);

/**
 * print_tokens()
 * Prints each token string and their frequencies
 * 
 * @pre         tokenList is null terminated
 * @param       TokenList*              the pointer to the token list
 */
void print_tokens(TokenList *tokenList);

/**
 * destroy_tokenList()
 * Cleans up dynamically-allocated resources including the list and its 
 * corresponding nodes
 * 
 * @pre         tokenList is null terminated
 * @param       TokenList*              the pointer to the token list
 */
void destroy_tokenList(TokenList *tl);

HashTable *create_hash_table();
void destroy_hash_table(HashTable *hashTable);
bool contains(HashTable *hashTable, char *item);
void print_elements(HashTable *hashTable);
int add_element(HashTable *hashTable, char *item);
unsigned long get_hash(char *key);

TokenList *tokenize_string(char *input, bool includeSpace);
void delete_token_strings(TokenList *tokenList);
void swap(TokenNode *a, TokenNode *b);
void sort_tokens(TokenList *tl);

#endif
