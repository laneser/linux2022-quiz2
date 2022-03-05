#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

size_t naive(uint64_t *bitmap, size_t bitmapsize, uint32_t *out)
{
    size_t pos = 0;
    for (size_t k = 0; k < bitmapsize; ++k)
    {
        uint64_t bitset = bitmap[k];
        size_t p = k * 64;
        for (int i = 0; i < 64; i++)
        {
            if ((bitset >> i) & 0x1)
                out[pos++] = p + i;
        }
    }
    return pos;
}

size_t improved(uint64_t *bitmap, size_t bitmapsize, uint32_t *out)
{
    size_t pos = 0;
    uint64_t bitset;
    for (size_t k = 0; k < bitmapsize; ++k)
    {
        bitset = bitmap[k];
        while (bitset != 0)
        {
            uint64_t t = bitset & -bitset;
            int r = __builtin_ctzll(bitset);
            out[pos++] = k * 64 + r;
            bitset ^= t;
        }
    }
    return pos;
}

size_t improved2(uint64_t *bitmap, size_t bitmapsize, uint32_t *out)
{
    size_t pos = 0;
    uint64_t bitset;
    for (size_t k = 0; k < bitmapsize; ++k)
    {
        bitset = bitmap[k];
        while (bitset != 0)
        {
            uint64_t t = bitset & -bitset;
            int r = __builtin_ctzll(bitset);
            out[pos++] = (k << 6) + r;
            bitset ^= t;
        }
    }
    return pos;
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
    int bitmapsize = 1024;
    int maxoutsize = bitmapsize * sizeof(uint64_t) * 8;
    uint64_t *bitmap = malloc(sizeof(uint64_t) * bitmapsize);
    uint32_t *naive_out = malloc(sizeof(uint32_t) * maxoutsize);
    uint32_t *improved_out = malloc(sizeof(uint32_t) * maxoutsize);
    uint32_t *improved2_out = malloc(sizeof(uint32_t) * maxoutsize);
    for (int i = 0; i < bitmapsize; i++)
        bitmap[i] = rand64();
    for (int i = 0; i < maxoutsize; i++)
        naive_out[i] = improved_out[i] = improved2_out[i] = 0;
    clock_t t1 = clock();
    naive(bitmap, bitmapsize, naive_out);
    clock_t t2 = clock();
    improved(bitmap, bitmapsize, improved_out);
    clock_t t3 = clock();
    improved2(bitmap, bitmapsize, improved2_out);
    clock_t t4 = clock();
    float naive_cost = (float)(t2 - t1) / CLOCKS_PER_SEC;
    float improved_cost = (float)(t3 - t2) / CLOCKS_PER_SEC;
    float improved2_cost = (float)(t4 - t3) / CLOCKS_PER_SEC;
    int isPass = 1;
    for (int i = 0; i < maxoutsize; i++)
    {
        if (naive_out[i] != improved_out[i])
        {
            printf("Mismatched at index %d, %u != %u\r\n", i, naive_out[i], improved_out[i]);
            isPass = 0;
            break;
        }
        if (naive_out[i] != improved2_out[i])
        {
            printf("Mismatched2 at index %d, %u != %u\r\n", i, naive_out[i], improved2_out[i]);
            isPass = 0;
            break;
        }
    }
    printf(isPass ? "Passed\r\n" : "Failed\r\n");
    printf("naive cost %f seconds, improved cost %f seconds,\r\nUplift performance %f%%\r\n",
           naive_cost, improved_cost, (1 / improved_cost - 1 / naive_cost) * 100 / (1 / naive_cost));
    printf("naive cost %f seconds, improved2 cost %f seconds,\r\nUplift performance %f%%\r\n",
           naive_cost, improved2_cost, (1 / improved2_cost - 1 / naive_cost) * 100 / (1 / naive_cost));
}