#ifndef DRIFT_RENDERER_H
#define DRIFT_RENDERER_H

typedef unsigned int shader;

#define MAX_RENDER_OBJECTS 1000

typedef struct texture
{
    u32 id;
    int width;
    int height;
    int channels;
} texture;

typedef enum rqi_type
{
    RENDER_TYPE_begin,
    RENDER_TYPE_submit,
    RENDER_TYPE_clear,
    RENDER_TYPE_viewport,

    RENDER_TYPE_line, 
    RENDER_TYPE_triangle,
    RENDER_TYPE_rect,
    RENDER_TYPE_texture,
    RENDER_TYPE_text,
} render_type;

typedef struct render_queue_item
{
    rqi_type type;
    u32 offset;
    u32 size; // TODO: Size might not me needed
} render_queue_item;

typedef struct render_queue
{
    render_queue_item items[MAX_RENDE_OBJECTS];
    u32 count;
    void *queue_data_buffer;
} render_queue;

typedef struct renderer
{
    int window_width;
    int window_height;
    matrix4f projection_matrix;
    
    shader shader;
    shader texture_shader;
    shader text_shader;
    font d_font;

    u32 vao;
    u32 vbo;
    u32 ebo;
    u32 texture_vao;
    u32 texture_vbo;
    u32 texture_ebo;


    render_queue queue;
} renderer;

#endif
