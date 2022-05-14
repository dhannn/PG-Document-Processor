#include "ceemply/ceemply.h"
#include "../src/pgdocs.h"
#include "../src/deps/internals.h"
#include <stdlib.h>

void test__add_element_setup(void **ptr)
{
    HashTable *hashTable = create_hash_table();
    *ptr = hashTable;
}

void test__add_element_destroy(void **ptr)
{
    HashTable **hashTable = (HashTable**)ptr;
    destroy_hash_table(*hashTable);
}

void test__add_element1(void *ptr)
{
    HashTable *hashTable = (HashTable*)ptr;

    int index = add_element(hashTable, "hello");

    char *expected = "hello";
    char *actual = hashTable[index].tokenList->head->tokenString;

    assert_equal_string(actual, expected);
}

void test__add_element2(void *ptr)
{
    HashTable *hashTable = (HashTable*)ptr;

    int index = add_element(hashTable, "hello");
    index = add_element(hashTable, "world");

    char *expected = "world";
    char *actual = hashTable[index].tokenList->head->next->tokenString;

    assert_equal_string(actual, expected);
}

void test__add_element3(void *ptr)
{
    HashTable *hashTable = (HashTable*)ptr;

    add_element(hashTable, "hello");
    add_element(hashTable, "world");
    add_element(hashTable, "!");
    add_element(hashTable, "!");
    int index = add_element(hashTable, "!");

    char *expected = "!";
    char *actual = hashTable[index].tokenList->head->next->next->next->next->tokenString;

    assert_equal_string(actual, expected);
}

void test__add_element()
{
    TestSuite *testSuite = create_test_suite(
        "add_element()", 
        test__add_element_setup, 
        test__add_element_destroy, 
        3
    );

    add_test(testSuite, test__add_element1);
    add_test(testSuite, test__add_element2);
    add_test(testSuite, test__add_element3);
    run_tests(testSuite);
    destroy_test_suite(testSuite);
}

void test__contains_setup(void **ptr)
{
    HashTable *hashTable = create_hash_table();
    *ptr = hashTable;
}

void test__contains_destroy(void **ptr)
{
    HashTable **hashTable = (HashTable**)ptr;
    destroy_hash_table(*hashTable);
}

void test__contains1(void *ptr)
{
    HashTable *hashTable = (HashTable*)ptr;

    add_element(hashTable, "hello");

    bool expected = true;
    bool actual = contains(hashTable, "hello");

    assert_equal_int(expected, actual);
}

void test__contains2(void *ptr)
{
    HashTable *hashTable = (HashTable*)ptr;

    add_element(hashTable, "hello");
    add_element(hashTable, "world");
    add_element(hashTable, "!");

    bool expected = true;
    bool actual = contains(hashTable, "!");

    assert_equal_int(expected, actual);
}

void test__contains3(void *ptr)
{
    HashTable *hashTable = (HashTable*)ptr;

    add_element(hashTable, "hello");
    add_element(hashTable, "world");
    add_element(hashTable, "!");
    
    bool expected = false;
    bool actual = contains(hashTable, " ");

    assert_equal_int(expected, actual);
}

void test__contains()
{
    TestSuite *testSuite = create_test_suite(
        "contains()",
        test__contains_setup,
        test__contains_destroy,
        3
    );

    add_test(testSuite, test__contains1);
    add_test(testSuite, test__contains2);
    add_test(testSuite, test__contains3);
    run_tests(testSuite);
    destroy_test_suite(testSuite);
}

int main()
{
    test__add_element();
    test__contains();
    return 0;
}
