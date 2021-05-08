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

    local_persist v2 test_pos = {0};
    local_persist v2 test_vel = {0};


    // NOTE: For testing purposes only
    {
        f32 move_speed = 15.f;

        // Movement
        if (platform->key_down[KEY_w] || platform->key_down[KEY_up])
        {
            test_vel.y += (-move_speed - test_vel.y);
        }
        if (platform->key_down[KEY_a] || platform->key_down[KEY_left])
        {
            test_vel.x += (-move_speed - test_vel.x);
        }
        if (platform->key_down[KEY_s] || platform->key_down[KEY_down])
        {
            test_vel.y += (move_speed - test_vel.y);
        }
        if (platform->key_down[KEY_d] || platform->key_down[KEY_right])
        {
            test_vel.x += (move_speed - test_vel.x);
        }

        // Update position
        if (test_vel.x != 0 && test_vel.y != 0)
        {
            // TODO: Maybe change Normalize to work on pointers
            test_vel = V2Normalize(test_vel);
            test_vel = V2Scalar(test_vel, move_speed);
        }

        local_persist b32 space = 0;
        if (!space && platform->key_down[KEY_space])
        {
            space = 1;
            test_pos.y -= 100;
        }

        test_pos.x += test_vel.x;
        test_pos.y += 9;

        if (!platform->key_down[KEY_space])
        {
            space = 0;
        }

        // World boundry collisions
        // TODO: Can change this to clamp
        if (test_pos.x <= 0)
        {
            test_pos.x = 0;
        }
        else if (test_pos.x >= platform->window_width - 32)
        {
            test_pos.x = platform->window_width - 32;
        }
            
        if (test_pos.y <= 0)
        {
            test_pos.y = 0;
        }
        else if (test_pos.y >= platform->window_height - 32)
        {
            test_pos.y = platform->window_height - 32;
        }

        // Reset velocity
        test_vel.x = 0;
        test_vel.y = 0;
    }

    ClearScreen(v4(0.2f, 0.3f, 0.3f, 1.0f));

    BeginRenderer(&state->renderer, platform->window_width, platform->window_height);

    RenderRect(&state->renderer, test_pos, v2(32.f, 32.f), v4(1.0f, 0.5f, 0.2f, 1.0f));
    RenderRect(&state->renderer, v2(100, 100), v2(100.f, 100.f), v4(0.5, 0, 0, 1));
    RenderTriangle(&state->renderer, v2(150, 200), v2(150, 300),
                   v2(300, 300), v4(0, 0, 1, 1));
    SubmitRenderer(&state->renderer);

    platform->SwapBuffers();
}

