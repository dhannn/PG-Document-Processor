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

    assert_equal_string(token, tl->head->tokenString);
    assert_equal_null_ptr(tl->head->next);
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

    TokenNode *current = tl->head;
    for(int i = 0; i < sizeOfOriginal; i++) {
        assert_equal_string(sample[i], current->tokenString);
        current = tl->head->next;
    }

    assert_equal_string(token, current->tokenString);
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

    TokenNode *current = tl->head;
    for(int i = 0; i < sizeOfOriginal; i++) {
        assert_equal_string(sample[i], current->tokenString);
        current = current->next;
    }

    assert_equal_string(token, current->tokenString);
    assert_equal_null_ptr(current->next);
}

void test__add_token4(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    char *token = "!";

    add_token(tl, token);

    assert_equal_string(token, tl->head->tokenString);
    assert_equal_null_ptr(tl->head->next);
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

    TokenNode *current = tl->head;
    for(int i = 0; i < sizeOfOriginal; i++) {
        assert_equal_string(sample[i], current->tokenString);
        current = current->next;
    }

    assert_equal_string(token, current->tokenString);
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

    TokenNode *current = tl->head;
    for(int i = 0; i < sizeOfOriginal; i++) {
        assert_equal_string(sample[i], current->tokenString);
        current = current->next;
    }

    assert_equal_string(token, current->tokenString);
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

    TokenNode *current = tl->head;
    for(int i = 0; i < size - 1; i++) {
        assert_equal_string(expected[i], current->tokenString);
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

    TokenNode *current = tl->head;
    for(int i = 0; i < size - 1; i++) {
        assert_equal_string(expected[i], current->tokenString);
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

    TokenNode *current = tl->head;
    for(int i = 0; i < size - 1; i++) {
        assert_equal_string(expected[i], current->tokenString);
        current = current->next;
    }

    assert_equal_null_ptr(current);
}

void test__remove_token4(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    char *removedToken = "world";

    remove_token(tl, removedToken);

    assert_equal_null_ptr(tl->head);
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

    TokenNode *current = tl->head;
    for(int i = 0; i < size; i++) {
        assert_equal_string(expected[i], current->tokenString);
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

void test__is_token_found_setup(void **ptr)
{
    TokenList *tl = initialize_tokenlist();

    add_token(tl, "Hello");
    add_token(tl, " ");
    add_token(tl, "world");
    add_token(tl, "!");
    add_token(tl, "\n");

    *ptr = tl;
}

void test__is_token_found_destroy(void **ptr)
{
    TokenList **tl = (TokenList**)ptr;
    destroy_tokenList(*tl);
}

void test__is_token_found1(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    char *token = "CCPROG";

    int expected = false;
    int actual = is_token_found(tl, token);

    assert_equal_int(expected, actual);
}

void test__is_token_found2(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    char *token = "Hello";

    int expected = true;
    int actual = is_token_found(tl, token);

    assert_equal_int(expected, actual);
}

void test__is_token_found3(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    char *token = "\n";

    int expected = true;
    int actual = is_token_found(tl, token);

    assert_equal_int(expected, actual);
}

void test__is_token_found4(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    char *token = "world";

    int expected = true;
    int actual = is_token_found(tl, token);

    assert_equal_int(expected, actual);
}

void test__is_token_found()
{
    TestSuite *testSuite = create_test_suite(
        "is_token_found()", 
        test__is_token_found_setup,
        test__is_token_found_destroy, 
        4
    );

    add_test(testSuite, test__is_token_found1);
    add_test(testSuite, test__is_token_found2);
    add_test(testSuite, test__is_token_found3);
    add_test(testSuite, test__is_token_found4);
    run_tests(testSuite);
    
    destroy_test_suite(testSuite);
}

void test__increment_token_frequency_setup(void** ptr)
{
    TokenList *tk = initialize_tokenlist();

    *ptr = tk;
}

void test__increment_token_frequency_destroy(void **ptr)
{
    TokenList **tk = (TokenList**)ptr;
    destroy_tokenList(*tk);
}

void test__increment_token_frequency1(void *ptr)
{
    TokenList *tk = (TokenList*)ptr;
    char *token = "hello";

    add_token(tk, token);
    increment_token_frequency(tk, token);

    int expected = 1;
    int actual = tk->head->frequency;

    assert_equal_int(expected, actual);
}

void test__increment_token_frequency2(void *ptr)
{
    TokenList *tk = (TokenList*)ptr;
    char *token = "hello";

    add_token(tk, token);
    tk->head->frequency = 1;
    increment_token_frequency(tk, token);

    int expected = 2;
    int actual = tk->head->frequency;

    assert_equal_int(expected, actual);
}

void test__increment_token_frequency3(void *ptr)
{
    TokenList *tk = (TokenList*)ptr;
    char *token = "hello";

    add_token(tk, token);
    tk->head->frequency = 7;
    increment_token_frequency(tk, token);

    int expected = 8;
    int actual = tk->head->frequency;

    assert_equal_int(expected, actual);
}

void test__increment_token_frequency4(void *ptr)
{
    TokenList *tk = (TokenList*)ptr;
    char *token[] = {"hello", "world", "!"};
    int tokenFreq[] = {0, 2, 11};

    TokenNode *curr;
    for(int i = 0; i < 3; i++) {
        add_token(tk, token[i]);

        if(tk->head->next == NULL)
            curr = tk->head;
        else
            curr = curr->next;
        
        curr->frequency = tokenFreq[i];        

    }
    
    increment_token_frequency(tk, token[1]);

    int expected = 3;
    int actual = tk->head->next->frequency;

    assert_equal_int(expected, actual);
}

void test__increment_token_frequency5(void *ptr)
{
    TokenList *tk = (TokenList*)ptr;
    char *token[] = {"hello", "world", "!"};
    int tokenFreq[] = {0, 2, 11};

    TokenNode *curr;
    for(int i = 0; i < 3; i++) {
        add_token(tk, token[i]);

        if(tk->head->next == NULL)
            curr = tk->head;
        else
            curr = curr->next;
        
        curr->frequency = tokenFreq[i];        

    }
    
    increment_token_frequency(tk, token[2]);

    int expected = 12;
    int actual = tk->head->next->next->frequency;

    assert_equal_int(expected, actual);
}

void test__increment_token_frequency()
{
    TestSuite *ts = create_test_suite(
        "increment_token_frequency()",
        test__increment_token_frequency_setup,
        test__increment_token_frequency_destroy,
        4
    );

    add_test(ts, test__increment_token_frequency1);
    add_test(ts, test__increment_token_frequency2);
    add_test(ts, test__increment_token_frequency3);
    add_test(ts, test__increment_token_frequency4);
    run_tests(ts);
    destroy_test_suite(ts);
}

void test__next_token_setup(void **ptr)
{
    TokenList *tl = initialize_tokenlist();
    add_token(tl, "hi");
    add_token(tl, "hello");
    add_token(tl, "world");

    *ptr = tl;
}

void test__next_token_destroy(void **ptr)
{
    TokenList **tl = (TokenList**)ptr;

    destroy_tokenList(*tl);
}

void test__next_token1(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    TokenNode *tokenNode = next_token(tl);

    assert_equal_string(tokenNode->tokenString, "hi");
}

void test__next_token2(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    next_token(tl);
    TokenNode *tokenNode = next_token(tl);

    assert_equal_string(tokenNode->tokenString, "hello");
}

void test__next_token3(void *ptr)
{
    TokenList *tl = (TokenList*)ptr;
    next_token(tl);
    next_token(tl);
    next_token(tl);
    TokenNode *tokenNode = next_token(tl);

    assert_equal_null_ptr(tokenNode);
}

void test__next_token()
{
    TestSuite *ts = create_test_suite(
        "next_token()", 
        test__next_token_setup,
        test__next_token_destroy, 
        3
    );

    add_test(ts, test__next_token1);
    add_test(ts, test__next_token2);
    add_test(ts, test__next_token3);
    run_tests(ts);
    destroy_test_suite(ts);
}

int main()
{
    // test__add_token();
    // test__remove_token();
    // test__is_token_found();
    // test__increment_token_frequency();
    test__next_token();
    return 0;
}
