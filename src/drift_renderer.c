#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "drift_renderer.h"

// TODO: Create shader from file
internal shader CreateShader(const char *vertex_shader_source,
                             const char *fragment_shader_source)
{
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
        DriftLogWarning("%s", info_log);
    }

    // Fragment Shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        DriftLogWarning("%s", info_log);
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
        DriftLogWarning("%s", info_log);
    }

    // Delete Shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return (shader)shader_program;
}

internal void UseShader(shader shader)
{
    glUseProgram(shader);
}

internal void DetachShader()
{
    glUseProgram(0);
}

// TODO: Create Texture from data
internal texture CreateTexture(char *filename)
{
    texture tex;
    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);

    // TODO: Add flags for texture GL options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(1);
    u8 *data = stbi_load(filename, &tex.width, &tex.height, &tex.channels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex.width, tex.height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        DriftLogWarning("stbi_load could not load texture");
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return tex;
}

internal void UploadMatrix4f(shader shader, matrix4f matrix, char *name)
{
    int location = glGetUniformLocation(shader, name); 
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix.elements[0][0]);
}

internal void Upload4f(shader shader, v4 vec, char *name)
{
    int location = glGetUniformLocation(shader, name);
    glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

internal void Upload1i(shader shader, u32 val, char *name)
{
    int location = glGetUniformLocation(shader, name);
    glUniform1i(location, val);
}
    
internal void ClearScreen(v4 color)
{
    glClearColor(color.r, color.g, color.b, color.a); 
    glClear(GL_COLOR_BUFFER_BIT);
}

internal void InitRenderer(renderer *renderer)
{
    if (!gladLoadGL()) 
    {
        // TODO: Logging
    }

    renderer->shader = CreateShader(default_vertex_shader,
                                    default_fragment_shader);

    renderer->texture_shader = CreateShader(texture_vertex_shader,
                                            texture_fragment_shader);

    float vertices[] = {
        -0.5f, -0.5f, 1.f, 1.f, 1.f, 1.f,
         0.5f, -0.5f, 1.f, 1.f, 1.f, 1.f,
         0.5f,  0.5f, 1.f, 1.f, 1.f, 1.f,
        -0.5f, -0.5f, 1.f, 1.f, 1.f, 1.f,
         0.5f,  0.5f, 1.f, 1.f, 1.f, 1.f,
        -0.5f,  0.5f, 1.f, 1.f, 1.f, 1.f
    };

    glGenVertexArrays(1, &renderer->vao);
    glBindVertexArray(renderer->vao);

    glGenBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                 vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    float texture_vertices[] = {
        // position     // texture coords
         0.5f,  0.5f,   1.f, 1.f, // top right
         0.5f, -0.5f,   1.f, 0.f, // bottom right
        -0.5f, -0.5f,   0.f, 0.f, // bottom left
        -0.5f,  0.5f,   0.f, 1.f  // top left 
    };

    u32 indices[] = {
        0, 1, 3,
        1, 2, 3
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

internal void BeginRenderer(renderer *renderer,
                            int window_width, int window_height)
{
    renderer->window_width = window_width;
    renderer->window_height = window_height;

    glViewport(0, 0, window_width, window_height);

    renderer->projection_matrix =
        OrthographicMatrix(0, window_width, window_height, 0, 0.f, 100.f);

    renderer->render_list_count = 0;
}

internal void SubmitRenderer(renderer *renderer)
{
    for (int i = 0; i < renderer->render_list_count; ++i)
    {
        render_object obj = renderer->render_list[i];
        switch(obj.type)
        {
            case RENDER_TYPE_line:
            {
                UploadMatrix4f(renderer->shader,
                               renderer->projection_matrix,
                               "projection");

                glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0,
                                12 * sizeof(float), obj.vertices);

                UseShader(renderer->shader);
                glBindVertexArray(renderer->vao);
                glDrawArrays(GL_LINE, 0, 3);
            } break;

            case RENDER_TYPE_triangle:
            {
                UploadMatrix4f(renderer->shader,
                               renderer->projection_matrix,
                               "projection");

                glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0,
                                18 * sizeof(float), obj.vertices);

                UseShader(renderer->shader);
                glBindVertexArray(renderer->vao);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            } break;

            case RENDER_TYPE_rect:
            {
                UploadMatrix4f(renderer->shader,
                               renderer->projection_matrix,
                               "projection");

                glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0,
                                36 * sizeof(float), obj.vertices);

                UseShader(renderer->shader);
                glBindVertexArray(renderer->vao);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            } break;

            case RENDER_TYPE_texture:
            {
                UploadMatrix4f(renderer->texture_shader,
                               renderer->projection_matrix,
                               "projection");

                // Get the data
                glBindBuffer(GL_ARRAY_BUFFER, renderer->texture_vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0,
                                16 * sizeof(float), obj.vertices);
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                // Specify Texture
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, obj.texture->id);

                Upload1i(renderer->texture_shader, 0, "tex");

                // Draw texture
                UseShader(renderer->texture_shader);
                glBindVertexArray(renderer->texture_vao);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                glBindVertexArray(0);
            } break;

            default: break;
        }
    }
}

internal void RenderLine(renderer *renderer, v2 p1, v2 p2, v4 color)
{
    render_object obj = {0};
    {
        obj.type = RENDER_TYPE_line;

        // Point 1
        obj.vertices[0] = p1.x;
        obj.vertices[1] = p1.y;

        obj.vertices[2] = color.r;
        obj.vertices[3] = color.g;
        obj.vertices[4] = color.b;
        obj.vertices[5] = color.a;

        // Point 2
        obj.vertices[6] = p2.x;
        obj.vertices[7] = p2.y;

        obj.vertices[8] = color.r;
        obj.vertices[9] = color.g;
        obj.vertices[10] = color.b;
        obj.vertices[11] = color.a;
    }

    renderer->render_list[renderer->render_list_count] = obj;
    renderer->render_list_count++;
}

internal void RenderTriangle(renderer *renderer, v2 p1,
                             v2 p2, v2 p3, v4 color)
{
    render_object obj = {0};
    {
        obj.type = RENDER_TYPE_triangle;

        // Bottom Left
        obj.vertices[0] = p1.x;
        obj.vertices[1] = p1.y;

        obj.vertices[2] = color.r;
        obj.vertices[3] = color.g;
        obj.vertices[4] = color.b;
        obj.vertices[5] = color.a;

        // Bottom Right
        obj.vertices[6] = p2.x;
        obj.vertices[7] = p2.y;

        obj.vertices[8] = color.r;
        obj.vertices[9] = color.g;
        obj.vertices[10] = color.b;
        obj.vertices[11] = color.a;
        
        // Top Right
        obj.vertices[12] = p3.x;
        obj.vertices[13] = p3.y;

        obj.vertices[14] = color.r;
        obj.vertices[15] = color.g;
        obj.vertices[16] = color.b;
        obj.vertices[17] = color.a;
    }

    renderer->render_list[renderer->render_list_count] = obj;
    renderer->render_list_count++;
}

internal void RenderRect(renderer *renderer, v2 position, v2 size, v4 color)
{
    render_object obj = {0};
    {
        obj.type = RENDER_TYPE_rect;

        v2 tl = position;
        v2 br = v2(position.x + size.x, position.y + size.y);

        // Bottom Left
        obj.vertices[0] = tl.x;
        obj.vertices[1] = br.y;

        obj.vertices[2] = color.r;
        obj.vertices[3] = color.g;
        obj.vertices[4] = color.b;
        obj.vertices[5] = color.a;

        // Bottom Right
        obj.vertices[6] = br.x;
        obj.vertices[7] = br.y;

        obj.vertices[8] = color.r;
        obj.vertices[9] = color.g;
        obj.vertices[10] = color.b;
        obj.vertices[11] = color.a;
        
        // Top Right
        obj.vertices[12] = br.x;
        obj.vertices[13] = tl.y;

        obj.vertices[14] = color.r;
        obj.vertices[15] = color.g;
        obj.vertices[16] = color.b;
        obj.vertices[17] = color.a;

        // Bottom Left
        obj.vertices[18] = tl.x;
        obj.vertices[19] = br.y;

        obj.vertices[20] = color.r;
        obj.vertices[21] = color.g;
        obj.vertices[22] = color.b;
        obj.vertices[23] = color.a;

        // Top Right
        obj.vertices[24] = br.x;
        obj.vertices[25] = tl.y;

        obj.vertices[26] = color.r;
        obj.vertices[27] = color.g;
        obj.vertices[28] = color.b;
        obj.vertices[29] = color.a;

        // Top Left
        obj.vertices[30] = tl.x;
        obj.vertices[31] = tl.y;

        obj.vertices[32] = color.r;
        obj.vertices[33] = color.g;
        obj.vertices[34] = color.b;
        obj.vertices[35] = color.a;
    }

    renderer->render_list[renderer->render_list_count] = obj;
    renderer->render_list_count++;
}

internal void RenderTexture(renderer *renderer, v2 position,
                            v2 size, texture *tex)
{
    render_object obj = {0};
    {
        obj.type = RENDER_TYPE_texture;
        obj.texture = tex;

        v2 tl = position;
        v2 br = v2(position.x + size.x, position.y + size.y);

        // Top Right
        obj.vertices[0] = br.x;
        obj.vertices[1] = tl.y;

        obj.vertices[2] = 1.f;
        obj.vertices[3] = 1.f;

        // Bottom Right
        obj.vertices[4] = br.x;
        obj.vertices[5] = br.y;

        obj.vertices[6] = 1.f;
        obj.vertices[7] = 0.f;

        // Bottom Left
        obj.vertices[8] = tl.x;
        obj.vertices[9] = br.y;

        obj.vertices[10] = 0.f;
        obj.vertices[11] = 0.f;

        // Top Left
        obj.vertices[12] = tl.x;
        obj.vertices[13] = tl.y;

        obj.vertices[14] = 0.f;
        obj.vertices[15] = 1.f;
    }

    renderer->render_list[renderer->render_list_count] = obj;
    renderer->render_list_count++;
}

