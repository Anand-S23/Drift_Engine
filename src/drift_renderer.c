#include <glad/glad.h>

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

