#include "internals.h"
#include "../pgdocs.h"

#include <stdlib.h>
#include <stdio.h>
#define SIZE 10000

unsigned long _get_hash_function(char *key);

HashTable *create_hash_table()
{
    HashTable *hashTable = malloc(sizeof(HashTable) * SIZE);

    for(int i = 0; i < SIZE; i++) {
        hashTable->tokens = NULL;
    }

    hashTable->size = 0;

    return hashTable;
}

void add_element(HashTable *hashTable, char *item)
{
    unsigned long index = _get_hash_function(item);
    
    TokenNode *tk = malloc(sizeof(TokenNode));
    
    add_token(tk, item);
}

void print_elements(HashTable *hashTable)
{
    return;
}

void destroy_hash_table(HashTable *hashTable)
{
    free(hashTable);
}

unsigned long _get_hash_function(char *key)
{
    unsigned long hash = 5381;
    int c = *key;

    while(c != '\0') {
        hash = ((hash << 5) + hash) + c;
        c = *key++;
    }

    return hash % SIZE;
}
