#ifndef DRIFT_RENDERER_H
#define DRIFT_RENDERER_H

#include "render_queue.h"

typedef unsigned int shader;

typedef struct texture
{
    u32 id;
    int width;
    int height;
    int channels;
} texture;

typedef struct renderer
{
    int window_width;
    int window_height;
    matrix4f projection_matrix;
    
    shader shader;
    shader texture_shader;
    shader text_shader;
    // TODO: font
    // font d_font;

    u32 vao;
    u32 vbo;
    u32 ebo;
    u32 texture_vao;
    u32 texture_vbo;
    u32 texture_ebo;

    render_queue queue;
} renderer;

#endif
