#pragma once

#ifndef _STDLIB_H
#define _STDLIB_H

#include "stdint.h"

// Just include necessary declarations - actual implementations are in string.c
char *itoa(int value, char *str, int base);

#endif /* _STDLIB_H */
