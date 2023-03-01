#include <drift.h>
#include <stdio.h>
#include <assert.h>

#include "app.h"

static app_t *app;

DRIFT_APP_ENTRY_POINT void init_app(drift_platform_t *platform)
{
    assert(sizeof(app_t) <= platform->permanent_storage_size);
    app = platform->permanent_storage;
    platform->initialized = 1;
}

DRIFT_APP_ENTRY_POINT void update_app(drift_platform_t *platform)
{
    app->x += 5;
    printf("%d\n", app->x);

    printf("%dms/f, %dfps, %dmc/f %lfs\n",
           platform->ms_per_frame,
           platform->fps,
           platform->mcpf,
           platform->delta_time);
}

DRIFT_APP_ENTRY_POINT drift_app_t drift_main(void)
{
    drift_app_t app = {
        .name = "Test Game",
        .window_width = 640,
        .window_height = 480
    };

    return app;
}



