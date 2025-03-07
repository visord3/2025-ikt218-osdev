#pragma once

#ifndef LIBC_STDINT_H
#define LIBC_STDINT_H

// Exact-width integer types
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

// Minimum-width integer types
typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;

typedef int8_t int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;

// Integer types capable of holding object pointers
typedef int32_t intptr_t;
typedef uint32_t uintptr_t;

// Size type
typedef uint32_t size_t;
typedef int32_t ssize_t;

// Boolean type
// typedef _Bool bool;
#define true 1
#define false 0

#endif /* LIBC_STDINT_H */
