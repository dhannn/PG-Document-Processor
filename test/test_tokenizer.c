#include "ceemply/ceemply.h"
#include "../src/pgdocs.h"
#include "../src/deps/internals.h"
#include <stdlib.h>

void test__add_token_setup(void **ptr)
{
    TokenList *tl = initialize_tokenlist();

    *ptr = tl;
}

void test__add_token_destroy(void **ptr)
{
    TokenList **tl = (TokenList**)ptr;
    destroy_tokenList(*tl);
}

void test__add_token1(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    char *token = "Hello";

    add_token(tl, token);

    assert_equal_string(token, tl->tokens->token);
    assert_equal_null_ptr(tl->tokens->next);
}

void test__add_token2(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    char *sample[] = {"Hello"};
    char *token = "world";
    int sizeOfOriginal = 1;

    for(int i = 0; i < sizeOfOriginal; i++) {
        add_token(tl, sample[i]);
    }

    add_token(tl, token);

    TokenNode *current = tl->tokens;
    for(int i = 0; i < sizeOfOriginal; i++) {
        assert_equal_string(sample[i], current->token);
        current = tl->tokens->next;
    }

    assert_equal_string(token, current->token);
    assert_equal_null_ptr(current->next);
}

void test__add_token3(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    char *sample[] = {"Hello", "world", "CCPROG", "Structured", "data"};
    char *token = "types";
    int sizeOfOriginal = 5;

    for(int i = 0; i < sizeOfOriginal; i++) {
        add_token(tl, sample[i]);
    }

    add_token(tl, token);

    TokenNode *current = tl->tokens;
    for(int i = 0; i < sizeOfOriginal; i++) {
        assert_equal_string(sample[i], current->token);
        current = current->next;
    }

    assert_equal_string(token, current->token);
    assert_equal_null_ptr(current->next);
}

void test__add_token4(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    char *token = "!";

    add_token(tl, token);

    assert_equal_string(token, tl->tokens->token);
    assert_equal_null_ptr(tl->tokens->next);
}

void test__add_token5(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    char *sample[] = {"CCPROG"};
    char *token = "2";
    int sizeOfOriginal = 1;

    for(int i = 0; i < sizeOfOriginal; i++) {
        add_token(tl, sample[i]);
    }

    add_token(tl, token);

    TokenNode *current = tl->tokens;
    for(int i = 0; i < sizeOfOriginal; i++) {
        assert_equal_string(sample[i], current->token);
        current = current->next;
    }

    assert_equal_string(token, current->token);
    assert_equal_null_ptr(current->next);
}

void test__add_token6(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    char *sample[] = {"Hello", "world"};
    char *token = "\n";
    int sizeOfOriginal = 2;

    for(int i = 0; i < sizeOfOriginal; i++) {
        add_token(tl, sample[i]);
    }

    add_token(tl, token);

    TokenNode *current = tl->tokens;
    for(int i = 0; i < sizeOfOriginal; i++) {
        assert_equal_string(sample[i], current->token);
        current = current->next;
    }

    assert_equal_string(token, current->token);
    assert_equal_null_ptr(current->next);
}

void test__add_token()
{
    TestSuite *testSuite = create_test_suite(
        "add_token()", 
        test__add_token_setup,
        test__add_token_destroy, 
        6
    );

    add_test(testSuite, test__add_token1);
    add_test(testSuite, test__add_token2);
    add_test(testSuite, test__add_token3);
    add_test(testSuite, test__add_token4);
    add_test(testSuite, test__add_token5);
    add_test(testSuite, test__add_token6);
    run_tests(testSuite);

    destroy_test_suite(testSuite);
}



void test__remove_token_setup(void **ptr)
{
    TokenList *tl = initialize_tokenlist();

    *ptr = tl;
}

void test__remove_token_destroy(void **ptr)
{
    TokenList **tl = (TokenList**)ptr;
    destroy_tokenList(*tl);
}

void test__remove_token1(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    char *tokens[] = {"Hello", "world", "!"};
    char *removedToken = "world";
    char *expected[] = {"Hello", "!"};
    int size = 3;

    for(int i = 0; i < size; i++) {
        add_token(tl, tokens[i]);
    }

    remove_token(tl, removedToken);

    TokenNode *current = tl->tokens;
    for(int i = 0; i < size - 1; i++) {
        assert_equal_string(expected[i], current->token);
        current = current->next;
    }

    assert_equal_null_ptr(current);
}

void test__remove_token2(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    char *tokens[] = {"Hello", "world", "!"};
    char *removedToken = "Hello";
    char *expected[] = {"world", "!"};
    int size = 3;

    for(int i = 0; i < size; i++) {
        add_token(tl, tokens[i]);
    }

    remove_token(tl, removedToken);

    TokenNode *current = tl->tokens;
    for(int i = 0; i < size - 1; i++) {
        assert_equal_string(expected[i], current->token);
        current = current->next;
    }

    assert_equal_null_ptr(current);
}

void test__remove_token3(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    char *tokens[] = {"Hello", "world", "!"};
    char *removedToken = "!";
    char *expected[] = {"Hello", "world"};
    int size = 3;


    for(int i = 0; i < size; i++) {
        add_token(tl, tokens[i]);
    }

    remove_token(tl, removedToken);

    TokenNode *current = tl->tokens;
    for(int i = 0; i < size - 1; i++) {
        assert_equal_string(expected[i], current->token);
        current = current->next;
    }

    assert_equal_null_ptr(current);
}

void test__remove_token4(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    char *removedToken = "world";

    remove_token(tl, removedToken);

    assert_equal_null_ptr(tl->tokens);
}

void test__remove_token5(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    char *tokens[] = {"Hello", "world", "!"};
    char *removedToken = "CCPROG";
    char *expected[] = {"Hello", "world", "!"};
    int size = 3;

    for(int i = 0; i < size; i++) {
        add_token(tl, tokens[i]);
    }

    remove_token(tl, removedToken);

    TokenNode *current = tl->tokens;
    for(int i = 0; i < size; i++) {
        assert_equal_string(expected[i], current->token);
        current = current->next;
    }

    assert_equal_null_ptr(current);
}

void test__remove_token()
{
    TestSuite *testSuite = create_test_suite(
        "remove_token()", 
        test__remove_token_setup,
        test__remove_token_destroy, 
        5
    );

    add_test(testSuite, test__remove_token1);
    add_test(testSuite, test__remove_token2);
    add_test(testSuite, test__remove_token3);
    add_test(testSuite, test__remove_token4);
    add_test(testSuite, test__remove_token5);
    run_tests(testSuite);

    destroy_test_suite(testSuite);
}


int main()
{
    test__add_token();
    test__remove_token();
    return 0;
}
