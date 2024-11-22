/*
** EPITECH PROJECT, 2024
** B-YEP-400-LYN-4-1-zappy-alexandre.douard
** File description:
** random.h
*/

#pragma once

#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#include "vector.h"

void start_random(unsigned seed);

uint64_t get_random(v2uint64_t range);