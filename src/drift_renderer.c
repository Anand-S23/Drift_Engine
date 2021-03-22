#include "drift_renderer.h"

internal shader CreateShader(const char *vertex_shader_source,
                             const char *fragment_shader_source)
{
    shader shader = {0};
    b32 success;
    char info_log[512];

    // Vertex  Shader
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        // TODO: Logging
    }

    // Fragment Shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        // TODO: Logging
    }

    // Link Shaders
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        // TODO: Logging
    }

    // Delete Shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    shader.program_id = shader_program;
    return shader;
}

internal void UseShader(shader shader)
{
    glUseProgram(shader.shader_program);
}

internal void DetachShader()
{
    glUseProgram(0);
}
    
// TODO: DeleteShader

internal void ClearScreen(v4 color)
{
    glClearColor(color.r, color.g, color.b, color.a); 
    glClear(GL_COLOR_BUFFER_BIT);
}

internal void InitRenderer(renderer *renderer)
{
    LoadAllOpenGLProcedures(platform);

    f32 vertices[18] = {0};

    glGenVertexArrays(1, &renderer->vao);
    glBindVertexArray(renderer->vao);

    glGenBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    renderer->shader = CreateShader(&defualt_vertex_shader,
                                    &default_fragment_shader);
}

internal void BeginRenderer(renderer *renderer,
                            int window_width, int window_height)
{
    renderer->window_width = window_width;
    renderer->window_height = window_height;

    // TODO: gl projection matrix (making opengl use screen coordinates)
    glViewport(0, 0, window_width, window_height);
}

internal void SubmitRenderer(renderer *renderer)
{
    for (int i = 0; i < renderer->render_list_count; ++i)
    {
        switch(renderer->render_list[i].type)
        {
            case RENDER_TYPE_rect:
            {
                glUseProgram(renderer->shader.program_id);

                // TODO: Update default

                glBindVertexArray(renderer->vao);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            } break;

            default:
            {
            } break;
        }
    }
            
}
    
