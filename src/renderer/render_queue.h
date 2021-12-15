#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

#define MAX_RENDER_OBJECTS 1000

typedef enum rqi_type
{
    RQI_TYPE_clear,
    RQI_TYPE_viewport,
    RQI_TYPE_line, 
    RQI_TYPE_triangle,
    RQI_TYPE_rect,
    RQI_TYPE_texture,
    RQI_TYPE_text
} rqi_type;

typedef struct render_queue_item
{
    rqi_type type;
    u8 data_count;
    u32 offset;
} render_queue_item;

typedef struct render_queue
{
    u32 count;
    render_queue_item items[MAX_RENDER_OBJECTS];
    u32 buffer_used;
    u32 buffer_size;
    void *buffer;
} render_queue;

#endif
