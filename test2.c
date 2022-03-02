#include <stdio.h>
#include <stdint.h>

typedef struct testfunc
{
    char *function_name;
    uint32_t (*test_function)(uint32_t, uint32_t);
} TestFunc;

typedef struct testfunc_signed
{
    char *function_name;
    int32_t (*test_function)(int32_t, int32_t);
} TestFunc_signed;

#include <stdint.h>
uint32_t max(uint32_t a, uint32_t b)
{
    return a ^ ((a ^ b) & -(a < b));
}

int32_t max_signed(int32_t a, int32_t b)
{
    return a ^ ((a ^ b) & -(a < b));
}

void runtest(uint32_t a, uint32_t b, uint32_t expect)
{
    TestFunc tests[] = {
        {"max", max},
    };

    for (int i = 0; i < sizeof(tests) / sizeof(TestFunc); i++)
    {
        int result = tests[i].test_function(a, b);
        if (result == expect)
        {
            printf("[V] %s(%u,%u)=%u\r\n", tests[i].function_name, a, b, result);
        }
        else
        {
            printf("[X] %s(%u,%u)=%u but expect %u\r\n", tests[i].function_name, a, b, result, expect);
        }
    }
}

void runtest_signed(int32_t a, int32_t b, int32_t expect)
{
    TestFunc_signed tests[] = {
        {"max_signed", max_signed},
    };

    for (int i = 0; i < sizeof(tests) / sizeof(TestFunc_signed); i++)
    {
        int result = tests[i].test_function(a, b);
        if (result == expect)
        {
            printf("[V] %s(%d,%d)=%d\r\n", tests[i].function_name, a, b, result);
        }
        else
        {
            printf("[X] %s(%d,%d)=%d but expect %u\r\n", tests[i].function_name, a, b, result, expect);
        }
    }
}

int main()
{
    runtest(0, 0, 0);
    runtest(0, 1, 1);
    runtest(1, 1, 1);
    runtest(3, 8, 8);
    runtest(9, 7, 9);
    runtest(7, 9, 9);
    runtest(UINT32_MAX, 0, UINT32_MAX);
    runtest(0, UINT32_MAX, UINT32_MAX);
    runtest(UINT32_MAX, UINT32_MAX, UINT32_MAX);

    runtest_signed(0, 0, 0);
    runtest_signed(0, -1, 0);
    runtest_signed(-1, 1, 1);
    runtest_signed(3, -8, 3);
    runtest_signed(9, 7, 9);
    runtest_signed(7, -9, 7);
    runtest_signed(INT32_MAX, 0, INT32_MAX);
    runtest_signed(-2, INT32_MAX, INT32_MAX);
    runtest_signed(INT32_MIN, INT32_MAX, INT32_MAX);
    runtest_signed(INT32_MAX, INT32_MAX, INT32_MAX);
    runtest_signed(INT32_MIN, INT32_MIN, INT32_MIN);
    runtest_signed(INT32_MIN, 5, 5);
    runtest_signed(INT32_MIN, -35, -35);
}