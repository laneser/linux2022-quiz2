#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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

// return 1 if gcd64_v2 equals gcd64, return 0 if failed.
int runtest(uint64_t u, uint64_t v)
{
    uint64_t result = gcd64_v2(u, v);
    uint64_t expect = gcd64(u, v);
    if (result == expect)
    {
        return 1;
    }
    else
    {
        printf("[X] gcd64(%lu,%lu)=%lu and gcd64_v2(%lu,%lu)=%lu\r\n", u, v, expect, u, v, result);
        return 0;
    }
}

uint64_t rand64(void)
{
    uint64_t r = 0;
    for (int i = 0; i < 64; i += 32)
    {
        r <<= 32;
        r ^= (unsigned)rand();
    }
    return r;
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
}