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

#define MOUSE_BUTTON_COUNT 4

enum
{
#define Key(name) KEY_##name,
#include "drift_key_list.h"
    KEY_MAX
#undef Key
};

enum
{
#define Button(name) BUTTON_##name,
    Button(up)
    Button(down)
    Button(left)
    Button(right)
    Button(start)
    Button(back)
    Button(left_thumb_stick)
    Button(right_thumb_stick)
    Button(left_shoulder)
    Button(right_shoulder)
    Button(a)
    Button(b)
    Button(x)
    Button(y)
    BUTTON_MAX
#undef Button
};

typedef struct input_state
{
    b32 begin_down;
    b32 is_down;
    b32 was_down;
} input_state_t;

typedef struct controller_input
{
    u8 controller_id;
    b32 is_connected;
    f32 left_stick_x;
    f32 left_stick_y;
    f32 right_stick_x;
    f32 right_stick_y;
    f32 left_trigger;
    f32 right_trigger;
    input_state_t buttons[BUTTON_MAX];
} controller_input_t;

typedef struct drift_platform
{
    b32 running;

    // Time
    f32 current_time;
    f32 last_time;
    f32 delta_time;

    // Input
    i16 wheel_delta;
    i32 mouse_x;
    i32 mouse_y;
    input_state_t left_mouse;
    input_state_t right_mouse;
    input_state_t middle_mouse;

    input_state_t keys[KEY_MAX];

    controller_input_t controller;
    controller_input_t controllers[4];
} drift_platform_t;

#endif // DRIFT_H
