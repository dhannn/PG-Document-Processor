/*******************************************************************************
 * 
 * FILE             hash_table.c
 * LAST MODIFIED    04-19-2022
 * 
 * DESCRIPTION
 *      This file contains function implementations that creates, modifies
 *      and handles hash tables.
 * 
 * ACKNOWLEDGMENTS 
 *      Stannis, Filip. (n.d.). 008 - djb2 hash | The Art in Code
 *      https://theartincode.stanis.me/008-djb2/
 *          This website contains the hash function we used for strings.
 * 
 ******************************************************************************/

#include "../pgdocs.h"
#include "internals.h"

#include <stdlib.h>
#include <stdio.h>
#define SIZE 10000

/* -------------------------------------------------------------------------- */
/*                         PRIVATE FUNCTION PROTOTYPES                        */
/* -------------------------------------------------------------------------- */
/* ----------------- functions only visible to hash_table.c ----------------- */
unsigned long __stub_get_hash();

/* -------------------------------------------------------------------------- */
/*                              PUBLIC FUNCTIONS                              */
/* -------------------------------------------------------------------------- */
/* ------------------ functions visible to outside modules ------------------ */
HashTable *create_hash_table()
{
    HashTable *hashTable = malloc(sizeof(HashTable) * SIZE);

    for(int i = 0; i < SIZE; i++)
        hashTable[i].tokenList = initialize_tokenlist();

    hashTable->size = 0;

    return hashTable;
}

bool contains(HashTable *hashTable, char *item)
{
    unsigned long index = get_hash(item);
    TokenList *tokens = hashTable[index].tokenList;

    return is_token_found(tokens, item);
}

int add_element(HashTable *hashTable, char *item)
{
    // we used the linked list implementation of the hash table 
    // to prevent collisions via chaining

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

unsigned long get_hash(char *key)
{
    // based on Daniel Bernstein's hash function, djb2
    unsigned long hash = 5381; 
    int c = *key;

    while(c != '\0') {
        hash = ((hash << 5) + hash) + c;
        c = *key++;
    }

    return hash % SIZE;
}

unsigned long __stub_get_hash()
{
    return 1;
}
