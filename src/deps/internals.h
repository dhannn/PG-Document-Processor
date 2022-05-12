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
    char *token;
    int frequency;
    TokenType tokenType;
    struct tk *next;
} TokenNode;

struct _tokenList {
    TokenNode *tokens;
    int size;
};

typedef struct _hashTable {
    TokenList *tokenList;
    int size;
} HashTable;

TokenList *initialize_tokenlist();
void remove_token(TokenList *tl, char *token);
void add_token(TokenList *tl, char *token);
void increment_token_frequency(TokenList *tokenList, char *token);
bool is_token_found(TokenList *tokenList, char *token);
void destroy_tokenList(TokenList *tl);
void print_tokens(TokenList *tokenList);

HashTable *create_hash_table();
void destroy_hash_table(HashTable *hashTable);
bool contains(HashTable *hashTable, char *item);
void print_elements(HashTable *hashTable);
int add_element(HashTable *hashTable, char *item);
unsigned long get_hash(char *key);

TokenList *tokenize(char *input, bool includeSpace);
void swap(TokenNode *a, TokenNode *b);
void sort_tokens(TokenList *tl);

#endif
