#ifndef DRIFT_COMMON_H
#define DRIFT_COMMON_H

#include <stdint.h>

#define UNUSED(x) (void)(x)

#define arr_size(arr) (sizeof(arr) / sizeof((arr)[0]))

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

#endif
