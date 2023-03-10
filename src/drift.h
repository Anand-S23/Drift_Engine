#ifndef DRIFT_H
#define DRIFT_H

#include <GL/gl.h>

#include "drift_common.h"
#include "drift_math.h"
#include "drift_renderer.c"

typedef struct read_file_result
{
    u32 size;
    void *memory;
} read_file_result_t;

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
    BUTTON_up,
    BUTTON_down,
    BUTTON_left,
    BUTTON_right,
    BUTTON_start,
    BUTTON_back,
    BUTTON_left_thumb_stick,
    BUTTON_right_thumb_stick,
    BUTTON_left_shoulder,
    BUTTON_right_shoulder,
    BUTTON_a,
    BUTTON_b,
    BUTTON_x,
    BUTTON_y,
    BUTTON_MAX
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
    // Options
    b32 running;

    // App Memory
    void *permanent_storage;
    u64 permanent_storage_size; 
    void *temp_storage; 
    u32 temp_storage_size;

    b32 initialized;

    // Time
    f32 current_time;
    f32 last_time;
    f32 delta_time;

    // TODO: Maybe not needed by the app
    i32 ms_per_frame;
    i32 fps;
    i32 mcpf;

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

    // IO
    void (*drift_free_file_memory)(void *memory);
    read_file_result_t (*drift_read_file)(const char *filename);
    b32 (*drift_write_file)(const char *filename, u32 memory_size, void *memory);

    // Renderer
    drift_renderer_t renderer;
    void (*drift_renderer_init)(drift_renderer_t *renderer);
    void (*drift_draw_test_rect)(drift_renderer_t *renderer);
} drift_platform_t;

typedef struct drift_app
{
    const char *name;
    int window_width;
    int window_height;
} drift_app_t;

#if defined(_MSC_VER)
    #define DRIFT_APP_ENTRY_POINT __declspec(dllexport)
#elif defined(__GNUC__)
    #define DRIFT_APP_ENTRY_POINT __attribute__((visibility("default")))
#else
    #define DRIFT_APP_ENTRY_POINT
#endif

typedef void init_app_t(drift_platform_t *platform);
typedef void update_app_t(drift_platform_t *platform);
typedef drift_app_t drift_main_t(void);

void init_app_stub(drift_platform_t *platform)
{
    platform->initialized = 0;
}

void update_app_stub(drift_platform_t *platform)
{
    platform->initialized = 0;
}

drift_app_t drift_main_stub(void)
{
    drift_app_t app = {0};
    return app;
}

#endif // DRIFT_H
