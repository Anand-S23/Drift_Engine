#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "app.h"
#include "drift_renderer.c"


internal drift_application DriftInit()
{
    drift_application app = {0};
    {
        strcpy(app.name, "Drift");
        app.window_width = 1280;
        app.window_height = 720;
    }

    return app;
}

internal void Update(platform *platform)
{
    Assert(sizeof(app_state) <= platform->storage_size);
    app_state *state = (app_state *)platform->storage;
    state->delta_t = platform->current_time - platform->last_time;

    if (!platform->initialized)
    {
        InitRenderer(&state->renderer);
        platform->initialized = 1;

        platform->LogWarning("Initialized platfrom");
    }


    ClearScreen(v4(0.2f, 0.3f, 0.3f, 1.0f));

    BeginRenderer(&state->renderer, platform->window_width, platform->window_height);
    RenderRect(&state->renderer, v2(0, 0), v2(100.f, 100.f), v4(1, 1, 1, 1));
    RenderRect(&state->renderer, v2(500, 100), v2(100.f, 200.f), v4(1, 1, 1, 1));
    RenderRect(&state->renderer, v2(100, 300), v2(100.f, 200.f), v4(1, 1, 1, 1));
    RenderRect(&state->renderer, v2(300, 300), v2(100.f, 200.f), v4(1, 1, 1, 1));
    RenderRect(&state->renderer, v2(1000, 1000), v2(100.f, 200.f), v4(1, 1, 1, 1));
    SubmitRenderer(&state->renderer);

    platform->SwapBuffers();
}

