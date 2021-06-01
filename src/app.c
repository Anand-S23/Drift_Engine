#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <drift.h>
#include "app.h"

global app_state *state;

INIT_APP
{
    Assert(sizeof(app_state) <= platform->storage_size);
    state = (app_state *)platform->storage;
    InitRenderer(&state->renderer);

    linked_list list = CreateList();
    PushType(&list, int, 5);
    PushType(&list, int, 1);
    AppendType(&list, int, 8);
    int x = *TopType(&list, int);

    int *z = RemoveType(&list, 5, int);

    state->tex = CreateTexture("W:\\drift_engine\\misc\\duck.png");
    state->mb = CreateTexture("W:\\drift_engine\\misc\\l.png");
    state->back = CreateTexture("W:\\drift_engine\\misc\\Background01.png");

    platform->initialized = 1;
}

UPDATE_APP
{
    state->delta_t = platform->current_time - platform->last_time;
    _DriftLog("Delta time: %lf", state->delta_t);

    local_persist v2 test_pos = {0};
    local_persist v2 test_vel = {0};


    // NOTE: For testing purposes only
    {
        f32 move_speed = 15.f;

        // Movement
        if (platform->keys[KEY_w].down || platform->keys[KEY_up].down)
        {
            test_vel.y += (-move_speed - test_vel.y);
        }
        if (platform->keys[KEY_a].down || platform->keys[KEY_left].down)
        {
            test_vel.x += (-move_speed - test_vel.x);
        }
        if (platform->keys[KEY_s].down || platform->keys[KEY_down].down)
        {
            test_vel.y += (move_speed - test_vel.y);
        }
        if (platform->keys[KEY_d].down || platform->keys[KEY_right].down)
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
        if (platform->keys[KEY_space].begin_down ||
            platform->controller.buttons[BUTTON_a].release)
        {
            space = 1;
            test_pos.y -= 100;
        }

        test_pos.x += test_vel.x;
        test_pos.y += 9;

        // World boundry collisions
        // TODO: Can change this to clamp
        if (test_pos.x <= 0)
        {
            test_pos.x = 0;
        }
        else if (test_pos.x >= platform->window_width - 32)
        {
            test_pos.x = (float)(platform->window_width - 32);
        }
            
        if (test_pos.y <= 0)
        {
            test_pos.y = 0;
        }
        else if (test_pos.y >= platform->window_height - 32)
        {
            test_pos.y = (float)(platform->window_height - 32);
        }

        // Reset velocity
        test_vel.x = 0;
        test_vel.y = 0;
    }

    ClearScreen(v4(0.2f, 0.3f, 0.3f, 1.0f));

    BeginRenderer(&state->renderer, platform->window_width, platform->window_height);

    // RenderTexture(&state->renderer, v2(0, 0), v2(1280, 720), &state->back);
    RenderTexture(&state->renderer, v2(100, 100), v2(256, 256), &state->tex);
    RenderTexture(&state->renderer, v2(0, 0), v2(256, 256), &state->mb);
    RenderRect(&state->renderer, test_pos, v2(32.f, 32.f), v4(1.0f, 0.5f, 0.2f, 1.0f));
    RenderRect(&state->renderer, v2(100, 100), v2(100.f, 100.f), v4(0.5, 0, 0, 1));
    RenderTriangle(&state->renderer, v2(150, 200), v2(150, 300),
                   v2(300, 300), v4(1, 0, 1, 0.5));

    RenderLine(&state->renderer, v2(0, 0), v2(300, 300), v4(0, 1, 0, 1));

    SubmitRenderer(&state->renderer);

    platform->SwapBuffers();
}

DRIFT_MAIN
{
    drift_application app = {0};
    {
        app.name = "Drift";
        app.window_width = 1280;
        app.window_height = 720;

        app.window_style = DWS_overlappedwindow | DWS_visible;
        app.window_exact = 1;
    }

    platform = platform_;
    return app;
}

