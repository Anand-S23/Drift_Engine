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
    Assert(sizeof(app_state) <= platform->permanent_storage_size);
    state = (app_state *)platform->permanent_storage;

    // App Memory
    state->permanent_arena = InitMemArena(memory->permanent_storage,
                                          memory->permanent_storage_size);
    state->temp_arena = InitMemArena(memory->temp_storage,
                                     memory->temp_storage_size);
    AllocateMemArena(&state->permanent_arena, sizeof(app_state));

    memory_sub_arena renderer_sub_arena =
        InitMemSubArena(state->permanent_arena, Megabytes(16));

    InitRenderer(&state->renderer);

    linked_list list = CreateList();
    PushType(&list, int, 5);
    PushType(&list, int, 1);
    AppendType(&list, int, 8);
    int x = *TopType(&list, int);

    int *z = RemoveType(&list, 5, int);

    if (!InitFont(&state->app_font, "W:\\drift_engine\\assets\\arial.ttf", 48))
    {
        DriftLogWarning("Error with initializg font");
    }

    texture_buffer buffer;
    InitTextureBuffer(&buffer, 256, 256);
    ClearTextureBuffer(&buffer,v4(0.f, 0.f, 0.f, 1.f));

    int color_count = 0;
    for (int j = 0; j < 2; ++j)
    {
        for (int i = 0; i < 2; ++i)
        {
            v4 color;
            color_count++;
            switch (color_count)
            {
                case 1:
                    color = v4(0.5, 0.5, 0.5, 1);
                    break;

                case 2:
                    color = v4(0.f, 0.f, 1.f, 1);
                    break;

                case 3:
                    color = v4(0.f, 1.f, 0.f, 1);
                    break;

                case 4:
                    color = v4(1.f, 0.f, 0.f, 1);
                    break;
            }

            RenderRectToBuffer(&buffer, v2(i * 128, j * 128), v2(128, 128), color);
        }
    }

    ReverseBuffer((u8 *)buffer.memory, buffer.width, buffer.height);
    texture test = CreateTextureFromData(buffer.memory, buffer.width, buffer.height);
    free(buffer.memory);

    texture text_tex = {0};
    text_tex.id = state->app_font.texture_atlas;
    text_tex.width = state->app_font.texture_size;
    text_tex.height = state->app_font.texture_size;
    text_tex.channels = 4;
    
    state->text = text_tex;
    state->tex = CreateTexture("W:\\drift_engine\\misc\\duck.png");
    state->tex = test;
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

#if 0
    RenderTexture(&state->renderer, v2(0, 0), v2(1280, 720), &state->back);
    RenderTexture(&state->renderer, v2(100, 100), v2(256, 256), &state->tex);
    RenderTexture(&state->renderer, v2(0, 0), v2(256, 256), &state->mb);
    RenderRect(&state->renderer, v2(100, 100), v2(100.f, 100.f), v4(0.5, 0, 0, 1));
    RenderTriangle(&state->renderer, v2(150, 200), v2(150, 300), v2(300, 300), v4(1, 0, 1, 0.5));
    RenderLine(&state->renderer, v2(0, 0), v2(300, 300), v4(0, 1, 0, 1));
    RenderTexture(&state->renderer, v2(400, 100),
                       v2(state->text.width, state->text.height),
                       &state->text);
#endif

    RenderTexture(&state->renderer, v2(100, 100), v2(300, 300), &state->tex);

    u32 tpc = PackRGBA(v4(1.f, 0.5f, 0.2f, 1.f));
    v4 tuc = UnpackRGBA(tpc);
    RenderRect(&state->renderer, test_pos, v2(32.f, 32.f), tuc);

    RenderText(&state->renderer, &state->app_font, "Yo it's working!", v2(100, 300), v3(1, 0, 0));

    local_persist ui app_ui = {0};
    UIBeginFrame(&app_ui, &state->renderer);
    {
        if (UIButton(&app_ui, UIIDGen(), "Test", v2(300, 0), v2(100, 50)))
        {
            RenderRect(&state->renderer, v2(100, 100),
                       v2(100.f, 100.f), v4(0.5, 0, 0, 1));
        }

        local_persist f32 value = 0.f;
        value = UISlider(&app_ui, UIIDGen(), v2(300, 75), v2(100, 50), value);

        if (UIMenu(&app_ui, UIIDGen(), "Test", v2(450, 10), v2(85, 45), v2(100, 50)))
        {
            UIMenuButton(&app_ui, UIIDGen(), "One");
            UIMenuButton(&app_ui, UIIDGen(), "Two");
            UIMenuButton(&app_ui, UIIDGen(), "Three");
        }
        UIEndMenu(&app_ui);
                
    }
    UIEndFrame(&app_ui);

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

