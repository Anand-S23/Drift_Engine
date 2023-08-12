#include <stddef.h>
#include "drift_renderer.h"

void drift_renderer_init(drift_renderer_t *renderer)
{
    u32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    // TODO: Check if compilation successfull

    u32 frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_shader_source, NULL);
    glCompileShader(frag_shader);
    // TODO: Check if compilation successfull

    renderer->shader_program = glCreateProgram();
    glAttachShader(renderer->shader_program, vertex_shader);
    glAttachShader(renderer->shader_program, frag_shader);
    glLinkProgram(renderer->shader_program);
    // TODO: check for success
    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

    glGenVertexArrays(1, &renderer->vao);
    glGenBuffers(1, &renderer->vbo);
    glGenBuffers(1, &renderer->ebo);
    glBindVertexArray(renderer->vao);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void drift_render_test(drift_renderer_t *renderer)
{
    glViewport(0, 0, 640, 480);
    glUseProgram(renderer->shader_program);
    glBindVertexArray(renderer->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
