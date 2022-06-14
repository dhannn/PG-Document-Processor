/*******************************************************************************
 * 
 * FILE             internals.h
 * LAST MODIFIED    04-19-2022
 * 
 * DESCRIPTION
 *      This file contains the function, structure and constants declarations
 *      of dependencies such as linked lists, hash tables and various
 *      utility/helper functions. 
 * 
 ******************************************************************************/

#include "../pgdocs.h"
#ifndef INTERNALS_H
#define INTERNALS_H
#include <stdbool.h>
#include <stdio.h>

typedef enum {
    ALPHA,          // alphabetic strings (e.g. "Hello", "wor", "d")
    NUMERIC,        // numeric characters (e.g. "1")
    WHITESPACE,     // whitespace characters (e.g. "\n", " ")
    SPECIAL         // characters not fitting to the previous ones 
                    // (e.g. "!", ","),
} TokenType;

typedef struct tk_n {
    char *tokenString;      // the string associated with the node
    int frequency;          // the frequency of the token
    float tfidf;
    TokenType tokenType;    // the type of the token
    struct tk_n *next;      // pointer for the next node
} TokenNode;

typedef struct {
    TokenNode *head;        // the first token in the list
    TokenNode *iterator;    // pointer to the current node
    int size;               // number of elements in the token list
} TokenList;

typedef struct _hashTable {
    TokenList *tokenList;   // a dynamic array of tokenlists
    int size;               // number of elements in the hash table
} HashTable;


/* -------------------------------------------------------------------------- */
/*                       FUNCTIONS FOR TOKEN LINKED LIST                      */
/* -------------------------------------------------------------------------- */

/**
 * initialize_tokenlist()
 * returns a pointer to an initialized token list
 * 
 * @return                              the pointer to the allocated list      
 */
TokenList *initialize_tokenlist();

/**
 * add_token()
 * adds a token to the linked list of tokens
 * 
 * @pre         linked list is null-terminated and initialized (i.e. not NULL)
 * @param       TokenList*              the pointer to the list to be added
 * @param       char*                   the token string to be added
 */
void add_token(TokenList *tl, char *token);

/**
 * remove_token()
 * removes the specified token from the list
 * 
 * @pre         linked list is null-terminated and initialized (i.e. not NULL)
 * @param       TokenList*              the pointer to the list to be removed
 * @param       char*                   the token string to be removed
 */
void remove_token(TokenList *tl, char *token);

/**
 * is_token_found()
 * checks if the token is in the linked list
 * 
 * @pre         linked list is null-terminated and initialized (i.e. not NULL)
 * @param       TokenList*              the pointer to the token list
 * @param       char*                   the token string to search
 * @return      bool                    true if found; false if not
 */
bool is_token_found(TokenList *tokenList, char *token);

/**
 * print_tokens()
 * prints each token string and their frequencies
 * 
 * @pre         linked list is null-terminated and initialized (i.e. not NULL)
 * @param       TokenList*              the pointer to the token list
 */
void print_tokens(TokenList *tokenList);

/**
 * next_token()
 * iterates through the token list
 * 
 * @pre         linked list is null-terminated and initialized (i.e. not NULL)
 * @param       TokenList*              the pointer to the list to be iterated
 */
TokenNode *next_token(TokenList *tokenList);

/**
 * get_token_string()
 * returns the string of a specified node
 * 
 * @pre         linked list is null-terminated and initialized (i.e. not NULL)
 * @param       TokenNode*              the pointer to the specified node
 * @return      char*                   the token string in the node
 */
char *get_token_string(TokenNode *tokenNode);

/**
 * increment_token_frequency()
 * increases the frequency of a token
 * 
 * @pre         linked list is null-terminated and initialized (i.e. not NULL)
 * @pre         token must exist in the list
 * @pre         token must also be unique (i.e. duplicates are removed)
 * @param       TokenList*              the pointer to the token list
 * @param       char*                   the token string to increment
 */
void increment_token_frequency(TokenList *tokenList, char *token);

/**
 * destroy_tokenList()
 * cleans up dynamically-allocated resources including the list and its 
 * corresponding nodes
 * 
 * @pre         linked list is null-terminated and initialized (i.e. not NULL)
 * @param       TokenList*              the pointer to the token list
 */
void destroy_tokenList(TokenList *tl);


/* -------------------------------------------------------------------------- */
/*                          FUNCTIONS FOR HASH TABLE                          */
/* -------------------------------------------------------------------------- */
/**
 * create_hash_table()
 * creates a hash table dynamically
 * 
 * @return      the pointer to the created hash table
 */
HashTable *create_hash_table();

/**
 * add_element()
 * adds an item to the hash table
 * 
 * @pre         hash table is initialized (i.e. not NULL)
 * @param       HashTable*      pointer of the hash table where item is added
 * @param       char*           the string to be added
 * @return                      the hash of the item
 */
int add_element(HashTable *hashTable, char *item);

/**
 * contains()
 * checks if an item is in the hash table
 * 
 * @pre         hash table is initialized (i.e. not NULL)
 * @pre         char pointer is valid
 * @param       HashTable*      pointer of the hash table to be searched
 * @return                      TRUE if item is found; FALSE if otherwise
 */
bool contains(HashTable *hashTable, char *item);

/**
 * print_elements()
 * prints the hash table and the corresponding linked lists
 * 
 * @pre         hash table is initialized (i.e. not NULL)
 * @param       HashTable*      pointer of the hash table to be printed
 * @note        This function is solely for testing functions under the module
 */
void print_elements(HashTable *hashTable);

/**
 * get_hash()
 * gets the hash of the key
 * 
 * @pre         char pointer is valid
 * @param       char*           the string the serves as the key
 * @return                      the hash of the specified key
 */
unsigned long get_hash(char *key);

/**
 * destroy_hash_table()
 * frees the allocated memory for the hash table
 * 
 * @pre         hash table is initialized (i.e. not NULL)
 * @param       HashTable*       pointer of the hash table to be destroyed
 */
void destroy_hash_table(HashTable *hashTable);

/* -------------------------------------------------------------------------- */
/*                               UTILS FUNCTIONS                              */
/* -------------------------------------------------------------------------- */

/**
 * tokenize_string()
 * separates a string to its corresponding tokens
 * 
 * @pre         char pointer is valid
 * @param       char*           the string to be tokenized
 * @param       bool            whether or not space is included in the tokens
 * @return                      a pointer to the tokenized string
 * 
 * @note        We defined tokens as either a string of purely alphabetic 
 *              characters or a single non-alphabetic character (e.g., "1", ".")
 */
TokenList *tokenize_string(char *input, bool includeSpace);

/**
 * remove_duplicates_tokens()
 * creates a tokenlist of unique values of a specified tokenlist
 * 
 * @pre         tokenlist is null-terminated and initialized (i.e. not NULL) 
 * @param       TokenList*      the pointer to the tokenlist to be processed
 * @return                      the pointer to the processed tokenlist 
 */
TokenList *remove_duplicate_tokens(TokenList*);

/**
 * create_string()
 * creates a dynamically-allocated string
 * 
 * @pre         char pointer is valid
 * @param       char*           the content of the allocated string
 * @return                      the newly-created string
 */
char *create_string(char *str);

/**
 * delete_token_strings()
 * frees the dynamically-allocated strings
 * 
 * @pre         tokenlist is null-terminated and initialized (i.e. not NULL) 
 * @param       TokenList*      the pointer to the tokenlist
 */
void delete_token_strings(TokenList *tokenList);

/**
 * swap()
 * swaps the contents of two nodes
 * 
 * @param       TokenNode*      pointer to the node to be swapped
 * @param       TokenNode*      pointer to the node to be swapped
 */
void swap(TokenNode *a, TokenNode *b);

/**
 * sort_tokens_by_freq()
 * sorts the tokens in the specified tokenlist by their frequency
 * 
 * @pre         tokenlist is null-terminated and initialized (i.e. not NULL) 
 * @param       TokenList*      pointer to the tokenlist
 */
void sort_tokens_by_freq(TokenList *tl);

void sort_tokens_by_tfidf(TokenList *tl);

/**
 * sort_tokens_by_alpha()
 * sorts the tokens in the specified tokenlist alphabetically
 * 
 * @pre         tokenlist is null-terminated and initialized (i.e. not NULL) 
 * @param       TokenList*      pointer to the tokenlist
 */
void sort_tokens_by_alpha(TokenList *tl);

/**
 * fprintf_metadata_item()
 * prints a metadata item to a file or a stream
 * 
 * @pre         file pointer is not NULL
 * @pre         char pointers should be valid and not NULL
 * @param       FILE*           file or the stream to write on
 * @param       char*           the metadata item to print
 * @param       char*           the data associated to the metadata item
 */
void fprint_metadata_item(FILE *file, const char *metadata, const char *data);
#endif
