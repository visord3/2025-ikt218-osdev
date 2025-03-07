#ifndef LIBC_STDDEF_H
#define LIBC_STDDEF_H

#include "stdint.h"

// Define NULL pointer
#ifndef NULL
#define NULL ((void *)0)
#endif

// Offset of a member in a structure
#define offsetof(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)

#endif /* LIBC_STDDEF_H */