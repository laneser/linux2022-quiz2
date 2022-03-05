#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef uint64_t (*gcd_function)(uint64_t, uint64_t);

uint64_t gcd64(uint64_t u, uint64_t v)
{
    if (!u || !v)
        return u | v;
    while (v)
    {
        uint64_t t = v;
        v = u % v;
        u = t;
    }
    return u;
}

uint64_t gcd64_v2(uint64_t u, uint64_t v)
{
    if (!u || !v)
        return u | v;
    int shift;
    for (shift = 0; !((u | v) & 1); shift++)
    {
        u /= 2, v /= 2;
    }
    while (!(u & 1))
        u /= 2;
    do
    {
        while (!(v & 1))
            v /= 2;
        if (u < v)
        {
            v -= u;
        }
        else
        {
            uint64_t t = u - v;
            u = v;
            v = t;
        }
    } while (v);
    return u << shift;
}

uint64_t gcd64_v3(uint64_t u, uint64_t v)
{
    if (!u || !v)
        return u | v;
    int u_ctz = __builtin_ctzll(u);
    int v_ctz = __builtin_ctzll(v);
    u = u >> u_ctz;
    v = v >> v_ctz;
    int shift = u_ctz > v_ctz ? v_ctz : u_ctz;
    do
    {
        v = v >> __builtin_ctzll(v);
        if (u < v)
        {
            v -= u;
        }
        else
        {
            uint64_t t = u - v;
            u = v;
            v = t;
        }
    } while (v);
    return u << shift;
}

// return 1 if gcd64_v2 equals gcd64, return 0 if failed.
int runtest(uint64_t u, uint64_t v)
{
    uint64_t result_v2 = gcd64_v2(u, v);
    uint64_t result_v3 = gcd64_v3(u, v);
    uint64_t expect = gcd64(u, v);
    if ((result_v2 == expect) && (result_v3 == expect))
    {
        return 1;
    }
    else
    {
        if (result_v2 != expect)
            printf("[X] gcd64(%lu,%lu)=%lu and gcd64_v2(%lu,%lu)=%lu\r\n", u, v, expect, u, v, result_v2);
        if (result_v3 != expect)
            printf("[X] gcd64(%lu,%lu)=%lu and gcd64_v3(%lu,%lu)=%lu\r\n", u, v, expect, u, v, result_v3);
        return 0;
    }
}

uint64_t rand64(void)
{
    uint64_t r = 0;
    for (int i = 0; i < 64; i += 32)
    {
        r <<= 32;
        r |= (unsigned)rand();
    }
    return r;
}

// return execute func times cost in seconds
float execute_cost(gcd_function func, int times)
{
    clock_t start = clock();
    while (times--)
    {
        func(rand64(), rand64());
    }
    clock_t end = clock();
    return (float)(end - start) / CLOCKS_PER_SEC;
}

int main()
{
    srand(time(NULL));
    int test_times = 10000;
    int ok = 1;
    while (test_times--)
    {
        ok = runtest(rand64(), rand64()) && ok;
    }
    if (ok)
    {
        printf("Passed.\r\n");
    }
    // test performance
    int performance_times = 1000000;
    float gcd64_v2_cost = execute_cost(gcd64_v2, performance_times);
    float gcd64_v3_cost = execute_cost(gcd64_v3, performance_times);
    printf("gcd64_v2 cost %f seconds, __builtin_ctz cost %f seconds,\r\nUplift performance %f%%\r\n",
           gcd64_v2_cost, gcd64_v3_cost, (1/gcd64_v3_cost - 1/gcd64_v2_cost)*100/(1/gcd64_v2_cost));
}