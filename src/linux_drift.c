#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL_opengl.h>

#include "drift.h"

b32 handle_event(SDL_Event *event)
{
    b32 should_quit = 0;
 
    switch(event->type)
    {
        case SDL_QUIT:
        {
            should_quit = 1;
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
            }
        } break;
    }

    return !should_quit;
}

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window *window = SDL_CreateWindow("Drift",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640, 480,
                                          SDL_WINDOW_OPENGL|
                                          SDL_WINDOW_SHOWN|
                                          SDL_WINDOW_RESIZABLE);

    if (window)
    {
        int running = 1;
        SDL_GLContext opengl_context = SDL_GL_CreateContext(window);
        if (opengl_context)
        {
            glewExperimental = GL_TRUE; 
            GLenum glew_error = glewInit();
            if( glew_error != GLEW_OK )
            {
                // TODO: Logging
            }

            while (running)
            {
                // Input
                SDL_Event event;
                while(SDL_PollEvent(&event))
                {
                    running = handle_event(&event);
                }

                // Update

                // Render
            }
        }
        else
        {
            // TODO: Logging
        }
    }
    else
    {
        // TODO: Logging
    }

    SDL_Quit();
    return 0;
}

