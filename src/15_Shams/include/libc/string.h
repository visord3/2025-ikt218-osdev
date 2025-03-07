#pragma once

#ifndef LIBC_STRING_H
#define LIBC_STRING_H

#include "../types.h"
#include <stdint.h>

void int_to_ascii(int n, char str[]);
void hex_to_ascii(int n, char str[]);
void reverse(char s[]);
int strcmp(const char s1[], const char s2[]);
uint32_t strlen(const char *str);
void backspace(char s[]);
void append(char s[], char n);
int starts_with(char *str, const char *prefix);

#endif // LIBC_STRING_H