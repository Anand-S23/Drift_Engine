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

typedef enum render_type
{
    RENDER_TYPE_line, 
    RENDER_TYPE_triangle,
    RENDER_TYPE_rect,
    RENDER_TYPE_texture,
    RENDER_TYPE_text
} render_type;

typedef enum rqi_type
{
    RQI_TYPE_begin_pass,
    RQI_TYPE_end_pass,
    RQI_TYPE_clear,
    RQI_TYPE_viewport,
    RQI_TYPE_render
} render_type;

typedef struct render_queue_item
{
    rqi_type type;
    u32 offset;
    u32 size; // TODO: Size might not me needed
} render_queue_item;

typedef struct render_queue
{
    u32 count;
    render_queue_item items[MAX_RENDE_OBJECTS];
    u32 buffer_used;
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
