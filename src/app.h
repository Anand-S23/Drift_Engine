#ifndef APP_H
#define APP_H

typedef struct app_state
{
    // memory_arena permanent_arena;
    // memory_arena transient_arena;

    // ui ui;
    renderer renderer;
    f32 delta_t;

    font app_font;
    texture text;
    texture tex;
    texture mb;
    texture back;

    texture_buffer buffer;
} app_state;

#endif
