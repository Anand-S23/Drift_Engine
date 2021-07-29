#include <glad/glad.h>

// Pushes item with no data onto queue
internal b32 RenderQueuePushItemOnly(renderer *renderer, rqi_type type)
{
    render_queue_item rqi = {0};
    {
        rqi.type = type;
    }

    renderer->queue.items[renderer->queue.count++] = rqi;
    return 1; 
}

// Pushes items with data onto queue
internal b32 RenderQueuePushItem(renderer *renderer, rqi_type type,
                                  void *buffer, u32 size)
{
    render_queue_item rqi = {0};
    {
        rqi.type = type;
        rqi.offset = renderer->queue.buffer_used;
        rqi.size = size;
    }

    if (renderer->queue.buffer_used + size < sizeof(items))
    {
        renderer->queue.items[renderer->queue.count++] = rqi;
        MemoryCopy((u8 *)renderer->queue + renderer->queue.buffer_used, buffer, size);
        renderer->queue.buffer_used += size;
        return 1; 
    }

    return 0;
}

// Initializes renderer 
internal void InitRenderer(renderer *renderer, memory_arena *arena)
{
    if (!gladLoadGL())
    {
        DriftLogError("Glad did not load");
    }

    // TODO: Add assest path to platform
    if (!InitFont(&renderer->d_font, "W:\\drift_engine\\assets\\arial.ttf", 18))
    {
        DriftLogWarning("Error with initializg font");
    }

    // Enable opacity
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    u32 indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    // Primitives
    renderer->shader = CreateShader(default_vertex_shader,
                                    default_fragment_shader);

    float vertices[] = {
        // position    // color
         0.5f,  0.5f,   1.f, 1.f, 1.f, 1.f, // top right
         0.5f, -0.5f,   1.f, 1.f, 1.f, 1.f, // bottom right
        -0.5f, -0.5f,   1.f, 1.f, 1.f, 1.f, // bottom left
        -0.5f,  0.5f,   1.f, 1.f, 1.f, 1.f  // top left
    };

    glGenVertexArrays(1, &renderer->vao);
    glBindVertexArray(renderer->vao);

    glGenBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                 vertices, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &renderer->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
                 indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    // Texture and Text
    renderer->texture_shader = CreateShader(texture_vertex_shader,
                                            texture_fragment_shader);

    renderer->text_shader = CreateShader(text_vertex_shader,
                                         text_fragment_shader);

    float texture_vertices[] = {
        // position     // texture coords
         0.5f,  0.5f,   1.f, 1.f, // top right
         0.5f, -0.5f,   1.f, 0.f, // bottom right
        -0.5f, -0.5f,   0.f, 0.f, // bottom left
        -0.5f,  0.5f,   0.f, 1.f  // top left 
    };

    glGenVertexArrays(1, &renderer->texture_vao);
    glBindVertexArray(renderer->texture_vao);

    glGenBuffers(1, &renderer->texture_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->texture_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texture_vertices),
                 texture_vertices, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &renderer->texture_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->texture_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
                 indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

// Clears entire screen to color that was passed in 
internal void ClearScreen(v4 color)
{
    // TODO: Add flags for advanced renderering
    RenderQueuePushItem(renderer, RENDERER_TYPE_clear,
                        (void *)color, sizeof(v4));
}

internal void BeginRenderer(renderer *renderer, int widow_width, int width_height)
{
    renderer->window_width = window_width;
    renderer->window_height = window_height;

    RenderQueuePushItemOnly(renderer, RENDERER_TYPE_begin_pass);
    
    v2 window_dimension = v2(window_width, window_height);
    RenderQueuePushItem(renderer, RENDERER_TYPE_viewport,
                        (void *)window_dimension, sizeof(v2));

    renderer->queue.count = 0;
}

internal void SubmitRenderer(renderer *renderer)
{
    RenderQueuePushItemOnly(renderer, RENDERER_TYPE_end_pass);

    // TODO: Test if this actually works, or if have to revert similar to old
    for (u32 i = 0; i < renderer->queue.count; ++i)
    {
        renderer_item item = renderer->queue.items[i];
        switch (item.type)
        {
            case RQI_TYPE_begin_pass:
            {
            } break;

            case RQI_TYPE_end_pass:
            {
                // TODO: Disable stuff here
            } break;

            case RQI_TYPE_viewport:
            {
                v2 window_dim = *(v2 *)((u8 *)renderer->queue + item.offset);
                glViewport(0, 0, window_dim.width, window_dim.height);
            } break;

            case RQI_TYPE_clear:
            {
                v4 color = *(v4 *)((u8 *)renderer->queue + item.offset);
                glClearColor(color.r, color.g, color.b, color.a); 
                glClear(GL_COLOR_BUFFER_BIT);
            } break;

            case RQI_TYPE_render:
            {
                render_type type = *(render_type *)((u8 *)renderer->queue + item.offset);
                switch (type)
                {
                    case RENDER_TYPE_line:
                    {
                    } break;

                    case RENDER_TYPE_triangle:
                    {
                    } break;

                    case RENDER_TYPE_rect:
                    {
                    } break;

                    case RENDER_TYPE_texture:
                    {
                    } break;

                    case RENDER_TYPE_text:
                    {
                    } break;

                    default: break;
                }
            } break;
        }

        default: break;
    }
}
