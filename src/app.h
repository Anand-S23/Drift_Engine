#ifndef APP_H
#define APP_H

#include "drift_renderer.h"

typedef struct app_state
{
    // memory_arena permanent_arena;
    // memory_arena transient_arena;

    // ui ui;
    renderer renderer;
    f32 delta_t;
} app_state;

#endif
