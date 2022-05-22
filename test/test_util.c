#include "ceemply/ceemply.h"
#include "../src/pgdocs.h"
#include "../src/deps/internals.h"
#include <stdlib.h>

void test__tokenize_setup(void **ptr)
{
    *ptr = NULL;
}

void test__tokenize_destroy(void **ptr)
{
    *ptr = NULL;
}

void test__tokenize1(void *ptr)
{
    char *input = "Hello";
    char *expected[] = {"Hello"};
    TokenList *tl = tokenize_string(input);

    TokenNode *tokenNode = tl->head;
    
    int i = 0;
    while(tokenNode != NULL) {
        assert_equal_string(expected[i], tokenNode->tokenString);

        tokenNode = tokenNode->next;
        i++;
    }

    delete_token_strings(tl);
}

void test__tokenize2(void *ptr)
{
    char *input = ",";
    char *expected[] = {","};
    TokenList *tl = tokenize_string(input);

    TokenNode *tokenNode = tl->head;
    
    int i = 0;
    while(tokenNode != NULL) {
        assert_equal_string(expected[i], tokenNode->tokenString);

        tokenNode = tokenNode->next;
        i++;
    }

    delete_token_strings(tl);
}

void test__tokenize3(void *ptr)
{
    char *input = "Hel1o";
    char *expected[] = {"Hel", "1", "o"};
    TokenList *tl = tokenize_string(input);

    TokenNode *tokenNode = tl->head;
    
    int i = 0;
    while(tokenNode != NULL) {
        assert_equal_string(expected[i], tokenNode->tokenString);

        tokenNode = tokenNode->next;
        i++;
    }

    delete_token_strings(tl);
}

void test__tokenize4(void *ptr)
{
    char *input = "Hello world";
    char *expected[] = {"Hello", " ", "world"};
    TokenList *tl = tokenize_string(input);

    TokenNode *tokenNode = tl->head;
    
    int i = 0;
    while(tokenNode != NULL) {
        assert_equal_string(expected[i], tokenNode->tokenString);

        tokenNode = tokenNode->next;
        i++;
    }

    delete_token_strings(tl);
}

void test__tokenize5(void *ptr)
{
    char *input = "Hello!";
    char *expected[] = {"Hello", "!"};
    TokenList *tl = tokenize_string(input);

    TokenNode *tokenNode = tl->head;
    
    int i = 0;
    while(tokenNode != NULL) {
        assert_equal_string(expected[i], tokenNode->tokenString);

        tokenNode = tokenNode->next;
        i++;
    }

    delete_token_strings(tl);
}

void test__tokenize6(void *ptr)
{
    char *input = "Hel1o, world!\t";
    char *expected[] = {"Hel", "1", "o", ",", " ", "world", "!", "\t"};
    TokenList *tl = tokenize_string(input);

    TokenNode *tokenNode = tl->head;
    
    int i = 0;
    while(tokenNode != NULL) {
        assert_equal_string(expected[i], tokenNode->tokenString);

        tokenNode = tokenNode->next;
        i++;
    }

    delete_token_strings(tl);
}

void test__tokenize()
{
    TestSuite *ts = create_test_suite(
        "tokenize()",
        test__tokenize_setup,
        test__tokenize_destroy, 
        6
    );

    add_test(ts, test__tokenize1);
    add_test(ts, test__tokenize2);
    add_test(ts, test__tokenize3);
    add_test(ts, test__tokenize4);
    add_test(ts, test__tokenize5);
    add_test(ts, test__tokenize6);
    run_tests(ts);

    destroy_test_suite(ts);
}

void test__swap_setup(void **ptr)
{
    TokenList *tokenList = initialize_tokenlist();
    *ptr = tokenList;
}

void test__swap_destroy(void **ptr)
{
    TokenList **tokenList = (TokenList**)ptr;
    destroy_tokenList(*tokenList);
}

void test__swap1(void *ptr)
{
    TokenList *tokenList = (TokenList*)ptr;
    add_token(tokenList, "Hello");
    tokenList->head->frequency = 2;
    add_token(tokenList, " ");
    add_token(tokenList, "world");
    tokenList->head->next->next->frequency = 10;

    swap(tokenList->head, tokenList->head->next->next);

    assert_equal_string("world", tokenList->head->tokenString);
    assert_equal_string("Hello", tokenList->head->next->next->tokenString);
    assert_equal_int(10, tokenList->head->frequency);
    assert_equal_int(2, tokenList->head->next->next->frequency);
}

void test__swap2(void *ptr)
{
    TokenList *tokenList = (TokenList*)ptr;
    add_token(tokenList, "Hello");
    tokenList->head->frequency = 2;
    add_token(tokenList, " ");
    add_token(tokenList, "world");
    tokenList->head->next->next->frequency = 10;

    swap(NULL, tokenList->head->next->next);

    assert_equal_string("world", tokenList->head->next->next->tokenString);
    assert_equal_int(10, tokenList->head->next->next->frequency);
}

void test__swap3(void *ptr)
{
    TokenList *tokenList = (TokenList*)ptr;
    add_token(tokenList, "Hello");
    tokenList->head->frequency = 2;
    add_token(tokenList, " ");
    add_token(tokenList, "world");
    tokenList->head->next->next->frequency = 10;

    swap(tokenList->head->next->next, NULL);

    assert_equal_string("world", tokenList->head->next->next->tokenString);
    assert_equal_int(10, tokenList->head->next->next->frequency);
}

void test__swap()
{
    TestSuite *testSuite = create_test_suite(
        "swap()",
        test__swap_setup,
        test__swap_destroy,
        3
    );

    add_test(testSuite, test__swap1);
    add_test(testSuite, test__swap2);
    add_test(testSuite, test__swap3);
    run_tests(testSuite);
    destroy_test_suite(testSuite);
}

int main()
{
    test__tokenize();
    test__swap();

    return 0;
}
