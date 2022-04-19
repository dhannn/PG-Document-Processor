#include "pgdocs.h"
#include "deps/internals.h"

int main()
{
    HashTable *hashTable = create_hash_table();

    add_element(hashTable, "Hello");
    add_element(hashTable, "world");
    add_element(hashTable, "CCPROG2");
    add_element(hashTable, "pneumonoultramicroscopicsilicovolcanoconiosis");

    destroy_hash_table(hashTable);

    return 0;
}
