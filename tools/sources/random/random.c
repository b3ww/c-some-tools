/*
** EPITECH PROJECT, 2024
** B-YEP-400-LYN-4-1-zappy-alexandre.douard
** File description:
** random.c
*/

#include "random.h"
#include "vector.h"

#include <stdio.h>

uint64_t get_random(v2uint64_t range)
{
    uint64_t res = rand() % (range.y - range.x);
    res += range.x;
    // printf("res = %ld\n", res);
    return res;
}

void start_random(unsigned seed)
{
    srand(seed);
}
