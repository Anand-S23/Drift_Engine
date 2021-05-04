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
    local_persist renderer renderer = {0};
    if (!platform->initialized)
    {
        InitRenderer(&renderer);
        platform->initialized = 1;

        platform->LogWarning("Initialized platfrom");
    }

    ClearScreen(v4(0.2f, 0.3f, 0.3f, 1.0f));

    BeginRenderer(&renderer, platform->window_width, platform->window_height);
    RenderRect(&renderer, v2(0, 0), v2(100.f, 100.f), v4(1, 1, 1, 1));
    RenderRect(&renderer, v2(500, 100), v2(100.f, 200.f), v4(1, 1, 1, 1));
    RenderRect(&renderer, v2(100, 300), v2(100.f, 200.f), v4(1, 1, 1, 1));
    RenderRect(&renderer, v2(300, 300), v2(100.f, 200.f), v4(1, 1, 1, 1));
    RenderRect(&renderer, v2(1000, 1000), v2(100.f, 200.f), v4(1, 1, 1, 1));
    SubmitRenderer(&renderer);

    platform->SwapBuffers();
}

