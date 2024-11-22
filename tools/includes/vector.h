/*
** EPITECH PROJECT, 2023
** zappy_server
** File description:
** vector.h
*/

#pragma once

#include <stdint.h>

#define VEC_2_TYPE(type)                        \
    typedef struct vec2_##type##_s {            \
        type x;                                 \
        type y;                                 \
    } v2##type;

#define VEC_3_TYPE(type)                        \
    typedef struct vec3_##type##_s {            \
        type x;                                 \
        type y;                                 \
        type z;                                 \
    } v3##type;

VEC_2_TYPE  (uint8_t)
VEC_2_TYPE  (uint16_t)
VEC_2_TYPE  (uint32_t)
VEC_2_TYPE  (uint64_t)
VEC_2_TYPE  (int8_t)
VEC_2_TYPE  (int16_t)
VEC_2_TYPE  (int32_t)
VEC_2_TYPE  (int64_t)
VEC_2_TYPE  (int)
VEC_2_TYPE  (double)

VEC_3_TYPE  (uint8_t)
VEC_3_TYPE  (uint16_t)
VEC_3_TYPE  (uint32_t)
VEC_3_TYPE  (uint64_t)
VEC_3_TYPE  (int8_t)
VEC_3_TYPE  (int16_t)
VEC_3_TYPE  (int32_t)
VEC_3_TYPE  (int64_t)
VEC_3_TYPE  (int)
VEC_3_TYPE  (double)

#define VEC2(type, x, y) ((v2##type){x, y})
#define VEC3(type, x, y, z) ((v3##type){x, y, z})