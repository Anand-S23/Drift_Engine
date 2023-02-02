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

typedef struct input_state
{
    b32 begin_down;
    b32 is_down;
    b32 was_down;
} input_state_t;

typedef enum keyboard
{
#define Key(name) KEY_##name,
#include "drift_key_list.h"
    KEY_MAX
#undef Key
} keyboard_t;

typedef struct drift_platform
{
    b32 running;
    f32 current_time;
    f32 last_time;
    // TODO: delta_time

    input_state_t keys[KEY_MAX];
} drift_platform_t;

#endif // DRIFT_H
