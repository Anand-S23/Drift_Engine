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
    glUseProgram(shader.program_id);
}

internal void DetachShader()
{
    glUseProgram(0);
}

internal void ClearScreen(v4 color)
{
    glClearColor(color.r, color.g, color.b, color.a); 
    glClear(GL_COLOR_BUFFER_BIT);
}

internal void InitRenderer(renderer *renderer)
{
    renderer->shader = CreateShader(default_vertex_shader,
                                    default_fragment_shader);

    float vertices[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f

    };

    glGenVertexArrays(1, &renderer->vao);
    glBindVertexArray(renderer->vao);

    glGenBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                 vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

internal void BeginRenderer(renderer *renderer,
                            int window_width, int window_height)
{
    renderer->window_width = window_width;
    renderer->window_height = window_height;

    glViewport(0, 0, window_width, window_height);

    renderer->projection_matrix = OrthographicMatrix(0, window_width,
                                                     window_height, 0,
                                                     0.f, 100.f);
    renderer->render_list_count = 0;
}

internal void UploadMatrix4f(shader shader, matrix4f matrix, char *name)
{
    int location = glGetUniformLocation(shader.program_id, name); 
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix.elements[0][0]);
}
    
internal void SubmitRenderer(renderer *renderer)
{
    for (int i = 0; i < renderer->render_list_count; ++i)
    {
        render_object obj = renderer->render_list[i];
        switch(obj.type)
        {
            case RENDER_TYPE_rect:
            {
                UploadMatrix4f(renderer->shader,
                               renderer->projection_matrix,
                               "projection");

                glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(obj.vertices), obj.vertices);

                UseShader(renderer->shader);
                glBindVertexArray(renderer->vao);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            } break;

            default:
                break;
        }
    }
}

internal void RenderRect(renderer *renderer, v2 position, v2 size, v4 color)
{
    render_object obj = {0};
    {
        obj.type = RENDER_TYPE_rect;
        obj.position = position;
        obj.size = size;
        obj.color = color;

        v2 tl = position;
        v2 br = v2(position.x + size.x, position.y + size.y);

        obj.vertices[0] = tl.x;
        obj.vertices[1] = br.y;
        obj.vertices[2] = br.x;
        obj.vertices[3] = br.y;
        obj.vertices[4] = br.x;
        obj.vertices[5] = tl.y;

        obj.vertices[6] = tl.x;
        obj.vertices[7] = br.y;
        obj.vertices[8] = br.x;
        obj.vertices[9] = tl.y;
        obj.vertices[10] = tl.x;
        obj.vertices[11] = tl.y;
    }

    renderer->render_list[renderer->render_list_count] = obj;
    renderer->render_list_count++;
}
    
