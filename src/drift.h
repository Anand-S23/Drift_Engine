#ifndef DRIFT_H
#define DRIFT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

// Signed Integers
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

// Unsigned Integers
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// Booleans
typedef int8_t   b8;
typedef int16_t  b16;
typedef int32_t  b32;
typedef int64_t  b64;

// Floats
typedef float    f32;
typedef double   f64;

typedef struct drift_platform_t
{
    b32 running;
} drift_platform_t;

#endif // DRIFT_H
