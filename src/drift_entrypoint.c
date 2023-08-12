#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "drift.h"

#if defined(_WIN32) || defined(_WIN64)
    #include "win32_drift.c"
#elif defined(__LINUX__)
    #include "linux_drift.c"
#elif defined(__APPLE__)
    // TODO: Support apple
#endif

drift_platform_t global_platform = {0};

static void drift_load_app_defaults(drift_app_t *drift_app)
{
    drift_app->name = "Drift App";
    drift_app->window_width = 640;
    drift_app->window_height = 480;
}

static f32 drift_get_elapsed_time(u64 previous_counter, u64 current_counter)
{
    f32 performance_freq = (f32)SDL_GetPerformanceFrequency();
    return ((f32)(current_counter - previous_counter) / performance_freq);
}

static int drift_get_window_refresh_rate(SDL_Window *window)
{
    int fallback_refresh_rate = 60;

    SDL_DisplayMode mode;
    int window_index = SDL_GetWindowDisplayIndex(window);
    int display_mode_result = SDL_GetDesktopDisplayMode(window_index, &mode);
    b32 is_result_valid = (display_mode_result == 0 && mode.refresh_rate > 0);

    return is_result_valid ? mode.refresh_rate : fallback_refresh_rate;
}

static void drift_reset_frame_based_input(drift_platform_t *platform)
{
    // Mouse wheel
   platform->wheel_delta = 0;
    
    // Mouse
    platform->left_mouse.begin_down = 0;
    platform->left_mouse.was_down = 0;

    platform->middle_mouse.begin_down = 0;
    platform->middle_mouse.begin_down = 0;

    platform->right_mouse.was_down = 0;
    platform->right_mouse.was_down = 0;

    // Keyboard
    for (int i = 0; i < (int)KEY_MAX; ++i)
    {
        platform->keys[i].begin_down = 0;
        platform->keys[i].was_down = 0;
    }
}
    
static void drift_handle_event(drift_platform_t *platform, SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_QUIT:
        {
            platform->running = 0;
        } break;

        case SDL_MOUSEWHEEL:
        {
            platform->wheel_delta = event->wheel.y;
        } break;

        case SDL_MOUSEMOTION:
        {
            SDL_GetMouseState(&platform->mouse_x, &platform->mouse_y);
        } break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        {
            b32 is_down = (event->type == SDL_MOUSEBUTTONDOWN);
            u8 button_index = event->button.button;

            input_state_t *mouse[MOUSE_BUTTON_COUNT] = {
                NULL,
                &platform->left_mouse, // SDL_BUTTON_LEFT = 1
                &platform->middle_mouse, // SDL_BUTTON_MIDDLE = 2
                &platform->right_mouse // SDL_BUTTON_RIGHT = 3
            };

            if (button_index > 0 && button_index < MOUSE_BUTTON_COUNT)
            {
                mouse[button_index]->begin_down = is_down;
                mouse[button_index]->is_down = is_down;
                mouse[button_index]->was_down = !is_down; 
            }
        } break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
        {
            SDL_Keycode key_code = event->key.keysym.sym;
            u8 key_state = event->key.state;
            b32 key_repeat = !!event->key.repeat;

            b32 is_down = (key_state == SDL_PRESSED);
            b32 was_down = (key_state == SDL_RELEASED || key_repeat);
            u32 key_index = 0;

            if (!key_repeat)
            {
                if (key_code >= SDLK_a && key_code <= SDLK_z)
                {
                    key_index = KEY_a + (key_code - SDLK_a);
                }
                else if (key_code >= SDLK_0 && key_code <= SDLK_9)
                {
                    key_index = KEY_0 + (key_code - SDLK_0);
                }
                else if (key_code >= SDLK_F1 && key_code <= SDLK_F12)
                {
                    key_index = KEY_f1 + (key_code - SDLK_F1);
                }
                else
                {
                    switch (key_code)
                    {
                        case SDLK_ESCAPE: { key_index = KEY_esc; } break;
                        case SDLK_BACKSPACE: { key_index = KEY_backspace; } break;
                        case SDLK_DELETE: { key_index = KEY_delete; } break;
                        case SDLK_TAB: { key_index = KEY_tab; } break;
                        case SDLK_SPACE: { key_index = KEY_space; } break;
                        case SDLK_RETURN: { key_index = KEY_enter; } break;
                        case SDLK_LCTRL: { key_index = KEY_ctrl; } break;
                        case SDLK_RCTRL: { key_index = KEY_ctrl; } break;
                        case SDLK_LSHIFT: { key_index = KEY_shift; } break;
                        case SDLK_RSHIFT: { key_index = KEY_shift; } break;
                        case SDLK_LALT: { key_index = KEY_alt; } break;
                        case SDLK_RALT: { key_index = KEY_alt; } break;
                        case SDLK_UP: { key_index = KEY_up; } break;
                        case SDLK_DOWN: { key_index = KEY_down; } break;
                        case SDLK_LEFT: { key_index = KEY_left; } break;
                        case SDLK_RIGHT: { key_index = KEY_right; } break;
                        default: { SDL_Log("Key not supported: %d", key_code); } break;
                    }
                }

                platform->keys[key_index].is_down = is_down;
                platform->keys[key_index].begin_down = is_down;
                platform->keys[key_index].was_down = was_down;
            }

            b32 alt_key_was_down = (event->key.keysym.mod & KMOD_ALT);
            if (key_code == SDLK_F4 && alt_key_was_down)
            {
                platform->running = 0;
            }
        } break;

        default: break;
    }
}


int main(void)
{
    // Init SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC);

    // Create window
    drift_app_t drift_app;
    char *dll_path = drift_platform_get_dll_path();
    platform_app_code_t app_code = drift_platform_load_app_code(dll_path);
    if (app_code.is_valid)
    {
        drift_app = app_code.drift_main();
    }
    else
    {
        drift_load_app_defaults(&drift_app);
    }

    // TODO: Add more fields into drift_app_t
    u32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    SDL_Window *window = SDL_CreateWindow(drift_app.name,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          drift_app.window_width,
                                          drift_app.window_height,
                                          window_flags);

    if (window == NULL)
    {
        // TODO: Logging
        return 1;
    }

    // Init OpenGL
    int major, minor;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
    SDL_Log("GL version %d.%d", major, minor);

    if (SDL_GL_CreateContext(window) == NULL)
    {
        // TODO: Logging
        return 1;
    }

    // Initalize Global Platform
    global_platform.permanent_storage_size = Megabytes(64); 
    global_platform.permanent_storage = drift_platform_allocate_memory(
        global_platform.permanent_storage_size);

    global_platform.temp_storage_size = Megabytes(100);
    global_platform.temp_storage = drift_platform_allocate_memory(
        global_platform.temp_storage_size);

    global_platform.drift_free_file_memory = drift_platform_free_file_memory;
    global_platform.drift_read_file = drift_platform_read_file;
    global_platform.drift_write_file = drift_platform_write_file;

    int refresh_rate = drift_get_window_refresh_rate(window);
    int game_update_hz = refresh_rate / 2;
    f32 target_spf = 1.0f / (f32)game_update_hz;
    u64 preformance_freq = SDL_GetPerformanceFrequency();
    u64 previous_counter = SDL_GetPerformanceCounter();
    u64 last_cycle_count = _rdtsc();

    global_platform.last_time = global_platform.current_time;
    global_platform.current_time += (f32)target_spf;
    global_platform.delta_time = global_platform.current_time - global_platform.last_time;

    // Core loop
    global_platform.running = 1;
    app_code.init(&global_platform);
    while (global_platform.running)
    {
        SDL_Event event;
        drift_reset_frame_based_input(&global_platform);
        while(SDL_PollEvent(&event))
        {
            drift_handle_event(&global_platform, &event);
        }

        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        app_code.update(&global_platform);

        SDL_GL_SwapWindow(window); 

        // Update Time
        u64 current_counter = SDL_GetPerformanceCounter();
        f32 elapsed_time = drift_get_elapsed_time(previous_counter, current_counter);
        if (elapsed_time < target_spf)
        {
            int time_should_sleep = ((target_spf - elapsed_time) * 1000) - 1;
            if (time_should_sleep > 0)
            {
                SDL_Delay(time_should_sleep);
            }

            u64 updated_counter = SDL_GetPerformanceCounter();
            f32 frame_elapsed_time = drift_get_elapsed_time(previous_counter,
                                                            updated_counter);
            if(frame_elapsed_time < target_spf)
            {
                // TODO: Logging
                // TODO: Missed sleep
            }

            while(frame_elapsed_time < target_spf)
            {
                updated_counter = SDL_GetPerformanceCounter();
                frame_elapsed_time = drift_get_elapsed_time(previous_counter,
                                                            updated_counter);
            }
        }
        else
        {
            // TODO: Logging
            // TODO: Missed Frame
        }

        u64 end_counter = SDL_GetPerformanceCounter();
        u64 end_cycle_count = _rdtsc();

        u64 counter_elapsed = end_counter - previous_counter;
        u64 cycles_elapsed = end_cycle_count - last_cycle_count;

        i32 ms_per_frame = (i32)((1000 * counter_elapsed) / preformance_freq);
        i32 fps = (i32)(preformance_freq / counter_elapsed);
        i32 mcpf = (i32)(cycles_elapsed / (1000 * 1000));

        // TODO: might not be needed in platform
        global_platform.ms_per_frame = ms_per_frame;
        global_platform.fps = fps;
        global_platform.mcpf = mcpf;

        global_platform.last_time = global_platform.current_time;
        global_platform.current_time += 1.f / (f32)fps;
        global_platform.delta_time = global_platform.current_time - global_platform.last_time;

        previous_counter = end_counter;
        last_cycle_count = end_cycle_count;
    }

    free(dll_path);
    drift_platform_free_memory(global_platform.permanent_storage,
                               global_platform.permanent_storage_size);
    drift_platform_unload_app_code(&app_code);
    return 0;
}
