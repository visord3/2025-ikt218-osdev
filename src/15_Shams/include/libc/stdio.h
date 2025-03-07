#pragma once

#ifndef LIBC_STDIO_H
#define LIBC_STDIO_H

// Only include these once - remove the duplicates
#include "libc/stdint.h"
#include "libc/stddef.h"
#include "libc/stdbool.h"

// Print functions
bool print(const char *data, size_t length);
int putchar(int ic);
int printf(const char *__restrict__ format, ...);

// Kernel print functions
void kprintf(const char *format, ...);
void kputchar(char c);
void kputs(const char *str);

#endif /* LIBC_STDIO_H */