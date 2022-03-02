#include <stdio.h>
#include <stdint.h>

typedef struct testfunc
{
    char *function_name;
    uint32_t (*test_function)(uint32_t, uint32_t);
} TestFunc;

uint32_t average(uint32_t a, uint32_t b)
{
    return (a + b) / 2;
}

uint32_t average2(uint32_t a, uint32_t b)
{
    return a + (b - a) / 2;
}

uint32_t average_EXP1(uint32_t a, uint32_t b)
{
    return (a >> 1) + (b >> 1) + (a & b & 1);
}

uint32_t average_EXP2_EXP3(uint32_t a, uint32_t b)
{
    return (a & b) + ((a ^ b) >> 1);
}

void runtest(uint32_t a, uint32_t b, uint32_t expect)
{
    TestFunc tests[] = {
        {"average", average},
        {"average2", average2},
        {"average_EXP1", average_EXP1},
        {"average_EXP2_EXP3", average_EXP2_EXP3}};

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

int main()
{
    runtest(0, 0, 0);
    runtest(0, 1, 0);
    runtest(1, 1, 1);
    runtest(3, 8, 5);
    runtest(9, 7, 8);
    runtest(7, 9, 8);
    runtest(UINT32_MAX, 0, UINT32_MAX / 2);
    runtest(0, UINT32_MAX, UINT32_MAX / 2);
    runtest(UINT32_MAX, UINT32_MAX, UINT32_MAX);
}