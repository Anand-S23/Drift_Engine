#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL_opengl.h>

#include "drift.h"

drift_platform_t global_platform = {0};

static void handle_event(SDL_Event *event)
{
    switch(event->type)
    {
        case SDL_QUIT:
        {
            global_platform.running = 0;
        } break;

        case SDL_WINDOWEVENT:
        {
            switch(event->window.event)
            {
                case SDL_WINDOWEVENT_RESIZED:
                {
                } break;

                case SDL_WINDOWEVENT_FOCUS_GAINED:
                {
                } break;

                case SDL_WINDOWEVENT_EXPOSED:
                {
                } break;

                default: break;
            }
        } break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
        {
        } break;

        default: break;
    }
}


int main(void)
{
    // Init SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC);

    // Init OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create window
    // TODO: Get values from app, instead of hard coding
    u32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    SDL_Window *window = SDL_CreateWindow("Drift",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640, 480,
                                          window_flags);

    assert(window && "Window was not created");
    SDL_GLContext opengl_context = SDL_GL_CreateContext(window);
    glewExperimental = GL_TRUE; 
    GLenum glew_error = glewInit();

    if (!window || !opengl_context || glew_error != GLEW_OK)
    {
        // TODO: Logging
        return 1;
    }

    // Core loop
    global_platform.running = 1;
    while (global_platform.running)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            handle_event(&event);
        }

        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window); 

    }

    return 0;
}
