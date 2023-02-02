#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include "drift.h"

drift_platform_t global_platform = {0};

static void reset_input_begin_down(input_state_t *keys)
{
    for (int i = 0; i < (int)KEY_MAX; ++i)
    {
        keys[i].begin_down = 0;
    }
}
    
static void handle_event(SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_QUIT:
        {
            global_platform.running = 0;
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
                // TODO: Check for key press
                if (key_code >= SDLK_a && key_code <= SDLK_z)
                {
                    key_code = SDLK_a + (key_code - SDLK_a);
                }
                else if (key_code >= SDLK_0 && key_code <= SDLK_9)
                {
                    key_code = SDLK_0 + (key_code - SDLK_0);
                }
                else if (key_code >= SDLK_F1 && key_code <= SDLK_F12)
                {
                    key_code = SDLK_F1 + (key_code - SDLK_F1);
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

                global_platform.keys[key_index].is_down = is_down;
                global_platform.keys[key_index].begin_down = is_down;
                global_platform.keys[key_index].was_down = was_down;
            }


            b32 alt_key_was_down = (event->key.keysym.mod & KMOD_ALT);
            if (key_code == SDLK_F4 && alt_key_was_down)
            {
                global_platform.running = 0;
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
    // TODO: Get values from app, instead of hard coding
    u32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    SDL_Window *window = SDL_CreateWindow("Drift",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640, 480,
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
    printf("GL version %d.%d\n", major, minor);

    if (SDL_GL_CreateContext(window) == NULL)
    {
        // TODO: Logging
        return 1;
    }

    glewExperimental = GL_TRUE; 
    if (glewInit() != GLEW_OK)
    {
        // TODO: Logging
        return 1;
   }

    // Core loop

    /*
    // TODO: Dynamically get monitor refrest rate
    // TODO: Use get closet display mode based to screen width + height

    static int display_in_use = 0;
    int display_mode_count = SDL_GetNumDisplayModes(display_in_use);
    if (display_mode_count < 1)
    {
        // TODO: Logging
        return 1;
    }

    for (int i = 0; i < display_mode_count; ++i)
    {
        SDL_DisplayMode mode;
        if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0)
        {
            // TODO: Logging
            return 1;
        }

        SDL_Log("Mode %i\tbpp %i\t%s\t%i x %i",
                i, SDL_BITSPERPIXEL(mode.format),
                SDL_GetPixelFormatName(mode.format),
                mode.w, mode.h);
        SDL_Log("%d", mode.refresh_rate);
    }
    */
    int moniter_refresh_hz = 60;
    int game_update_hz = moniter_refresh_hz / 2;
    f32 target_fps = 1.0f / (f32)game_update_hz;

    global_platform.running = 1;
    while (global_platform.running)
    {
        u64 start_time = SDL_GetTicks();
        global_platform.last_time = global_platform.current_time;
        global_platform.current_time += 1.f / target_fps;

        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            reset_input_begin_down(&global_platform.keys);
            handle_event(&event);
        }

        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window); 
    }

    return 0;
}
