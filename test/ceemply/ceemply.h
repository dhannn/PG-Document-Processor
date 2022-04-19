#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdbool.h>

typedef struct _test_suite TestSuite;
typedef void(*Setup)(void**);
typedef void(*Test)(void*);
typedef void(*Destroy)(void**);

/* -------------------------------------------------------------------------- */

TestSuite *create_test_suite(char*, Setup, Destroy, int);
void destroy_test_suite(TestSuite*);

void add_test(TestSuite*, Test);
void run_tests(TestSuite*);

/* -------------------------------------------------------------------------- */

void assert_equal_int(int, int);
void assert_equal_null_ptr(void*);
void assert_equal_not_null_ptr(void*);
void assert_equal_string(const char *expected, const char *actual);

/* -------------------------------------------------------------------------- */

#endif