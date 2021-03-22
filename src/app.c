#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "app.h"
#include "drift_renderer.c"

renderer renderer = {0};

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
    if (!platform->initialized)
    {
        InitRenderer(&renderer);
        platform->initialized = 1;
    }

    ClearScreen(0.2f, 0.3f, 0.3f, 1.0f);

    BeginRenderer(&renderer, platform->window_width, platform->window_height);

    SubmitRenderer(&renderer);

    platform->SwapBuffers();
}

