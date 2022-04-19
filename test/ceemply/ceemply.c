#include "ceemply.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ASSERT(x, y) ((x) == (y))
#define DIVIDER "-------------------------------"
#define WIDTH 44

struct _test_suite {
    char *title;
    Setup setup;
    Test *tests;
    int numTests;
    Destroy destroy;
    void *vars;
};


TestSuite *create_test_suite(char* t, Setup s, Destroy d, int n)
{
    TestSuite *testSuite = malloc(sizeof(TestSuite)); 

    testSuite->title = t;
    testSuite->setup = s;
    testSuite->destroy = d;
    
    testSuite->numTests = n;
    testSuite->tests = malloc(n * sizeof(Test));

    for(int i = 0; i < n; i++) {
        testSuite->tests[i] = NULL;
    }

    return testSuite;
}

void add_test(TestSuite *testSuite, Test test)
{
    int i;
    for(i = 0; testSuite->tests[i] != NULL; i++);

    testSuite->tests[i] = test;
}

void run_tests(TestSuite* testSuite)
{
    // TODO: extract method
    int pad = (WIDTH - strlen("Test Suite: ") - strlen(testSuite->title)) / 2 + 1;
    printf("%*sTest Suite: %s%*s\n", pad, "", testSuite->title, pad, "");

    for(int i = 0; i < testSuite->numTests; i++) {
        testSuite->setup(&(testSuite->vars));

        pad = (WIDTH - strlen("Test Case # ")) / 2 + 1;
        printf("\n%.15s Test Case #%d %.15s\n",  DIVIDER, i + 1, DIVIDER);

        if(i < testSuite->numTests)
            printf("%10c", ' ');

        testSuite->tests[i](testSuite->vars);
        testSuite->destroy(&(testSuite->vars));
    }
}


void destroy_test_suite(TestSuite* testSuite)
{
    free(testSuite->tests);
    free(testSuite);
}

/* -------------------------------------------------------------------------- */

void assert_equal_int(int expected, int actual)
{
    char status = 'F';

    if(ASSERT(expected, actual))
        status = 'P';

    printf("\nExpected: %d\n", expected);
    printf("Actual: %d\n", actual);
    printf("Status: %c\n", status);
}

void assert_equal_string(const char *expected, const char *actual)
{
    char status = 'F';

    if(strcmp(expected, actual) == 0)
        status = 'P';

    printf("\nExpected: %s\n", expected);
    printf("Actual: %s\n", actual);
    printf("Status: %c\n", status);
}

void assert_equal_null_ptr(void *ptr)
{
    char status = 'F';

    if(ASSERT(ptr, NULL))
        status = 'P';

    printf("\nExpected: 0x0\n");
    printf("Actual: %p\n", ptr);
    printf("Status: %c\n", status);
}

void assert_equal_not_null_ptr(void *ptr)
{
    char status = 'F';

    if(!ASSERT(ptr, NULL))
        status = 'P';

    printf("\nExpected: not 0x0\n");
    printf("Actual: %p\n", ptr);
    printf("Status: %c\n", status);
}
