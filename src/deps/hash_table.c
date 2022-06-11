#include "../pgdocs.h"
#include "internals.h"

#include <stdlib.h>
#include <stdio.h>
#define SIZE 10000

unsigned long _stub_get_hash();

HashTable *create_hash_table()
{
    HashTable *hashTable = malloc(sizeof(HashTable) * SIZE);

    for(int i = 0; i < SIZE; i++) {
        hashTable[i].tokenList = initialize_tokenlist();
    }

    hashTable->size = 0;

    return hashTable;
}

bool contains(HashTable *hashTable, char *item)
{
    unsigned long index = get_hash(item);
    TokenList *tokens = hashTable[index].tokenList;

    return is_token_found(tokens, item);
}

// Pre-condition: item is unique
int add_element(HashTable *hashTable, char *item)
{
    unsigned long index = get_hash(item);
    add_token(hashTable[index].tokenList, item);
    return index;
}

void print_elements(HashTable *hashTable)
{
    for(int i = 0; i < SIZE; i++) {
        TokenNode *tn = hashTable[i].tokenList->head;

        printf("[%d]: ", i);

        while(tn != NULL) {
            printf("%s:%d", tn->tokenString, tn->frequency);

            tn = tn->next;

            if(tn != NULL)
                printf(" -> ");
        }

        printf("\n");
    }
}

void destroy_hash_table(HashTable *hashTable)
{
    for(int i = 0; i < SIZE; i++) {
        if(hashTable[i].tokenList != NULL)
            destroy_tokenList(hashTable[i].tokenList);
    }
    free(hashTable);
}

unsigned long _stub_get_hash()
{
    return 1;
}

unsigned long get_hash(char *key)
{
    // https://theartincode.stanis.me/008-djb2/

    unsigned long hash = 5381;
    int c = *key;

    while(c != '\0') {
        hash = ((hash << 5) + hash) + c; // hash =  hash * 33 + c
        c = *key++;
    }

    return hash % SIZE;
}
