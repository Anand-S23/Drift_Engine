#include "drift_queue.h"

// NOTE: The data is added to the queue buffer
//       in the same order as parameters

#define AppendDataToBuffer(queue, data, type) \
    MemoryCopy(queue->buffer + queue->buffer_used, data, sizeof(type)); \
    queue->buffer_used += sizeof(type);

internal b32 PushClear(render_queue *queue, v4 color)
{
    render_queue_item rqi = {0};
    {
        rqi.type = RQI_TYPE_clear;
        rqi.offset = queue->buffer_used;
        rqi.data_count = 4;
    }

    size_t item_size = sizeof(u8) * rqi.data_count;
    if (queue->buffer_size <= queue->buffer_used + item_size ||
        queue->count + 1 >= MAX_RENDER_OBJECTS)
    {
        return 0;
    }

    // Color
    AppendDataToBuffer(queue, (void *)&color.r, u8);
    AppendDataToBuffer(queue, (void *)&color.g, u8);
    AppendDataToBuffer(queue, (void *)&color.b, u8);
    AppendDataToBuffer(queue, (void *)&color.a, u8);

    queue->items[queue->count++] = rqi;
    return 1;
}

internal PushViewport(render_queue *queue, v2 pos, v2 size)
{
    render_queue_item rqi = {0};
    {
        rqi.type = RQI_TYPE_viewport;
        rqi.offset = queue->buffer_used;
        rqi.data_count = 4; 
    }

    size_t item_size = sizeof(u8) * rqi.data_count;
    if (queue->buffer_size <= queue->buffer_used + item_size ||
        queue->count + 1 >= MAX_RENDER_OBJECTS)
    {
        return 0;
    }

    // Viewport position
    AppendDataToBuffer(queue, (void *)&pos.x, f32);
    AppendDataToBuffer(queue, (void *)&pos.y, f32);

    // Viewport size
    AppendDataToBuffer(queue, (void *)&size.width, f32);
    AppendDataToBuffer(queue, (void *)&size.height, f32);

    queue->items[queue->count++] = rqi;
    return 1;
}

internal PushLine(render_queue *queue, v2 p1, v2 p2, v4 color)
{
    render_queue_item rqi = {0};
    {
        rqi.type = RQI_TYPE_line;
        rqi.offset = queue->buffer_used;
        rqi.data_count = 8;
    }

    size_t item_size = sizeof(u8) * rqi.data_count;
    if (queue->buffer_size <= queue->buffer_used + item_size ||
        queue->count + 1 >= MAX_RENDER_OBJECTS)
    {
        return 0;
    }

    // Line point 1
    AppendDataToBuffer(queue, (void *)&p1.x, f32);
    AppendDataToBuffer(queue, (void *)&p1.y, f32);

    // Line point 2
    AppendDataToBuffer(queue, (void *)&p2.x, f32);
    AppendDataToBuffer(queue, (void *)&p2.y, f32);

    // Line Color
    AppendDataToBuffer(queue, (void *)&color.r, u8);
    AppendDataToBuffer(queue, (void *)&color.g, u8);
    AppendDataToBuffer(queue, (void *)&color.b, u8);
    AppendDataToBuffer(queue, (void *)&color.a, u8);

    queue->items[queue->count++] = rqi;
    return 1;
}

internal PushTriangle(render_queue *queue, v2 p1, v2 p2, v2 p3, v4 color)
{
    render_queue_item rqi = {0};
    {
        rqi.type = RQI_TYPE_triangle;
        rqi.offset = queue->buffer_used;
        rqi.data_count = 10;
    }

    size_t item_size = sizeof(u8) * rqi.data_count;
    if (queue->buffer_size <= queue->buffer_used + item_size ||
        queue->count + 1 >= MAX_RENDER_OBJECTS)
    {
        return 0;
    }

    // Triangle point 1
    AppendDataToBuffer(queue, (void *)&p1.x, f32);
    AppendDataToBuffer(queue, (void *)&p1.y, f32);

    // Triangle point 2
    AppendDataToBuffer(queue, (void *)&p2.x, f32);
    AppendDataToBuffer(queue, (void *)&p2.y, f32);

    // Triangle point 2
    AppendDataToBuffer(queue, (void *)&p3.x, f32);
    AppendDataToBuffer(queue, (void *)&p3.y, f32);

    // Triangle Color
    AppendDataToBuffer(queue, (void *)&color.r, u8);
    AppendDataToBuffer(queue, (void *)&color.g, u8);
    AppendDataToBuffer(queue, (void *)&color.b, u8);
    AppendDataToBuffer(queue, (void *)&color.a, u8);

    queue->items[queue->count++] = rqi;
    return 1;
}

internal PushRectangle(render_queue *queue, v2 pos, v2 size, v4 color)
{
    render_queue_item rqi = {0};
    {
        rqi.type = RQI_TYPE_rect;
        rqi.offset = queue->buffer_used;
        rqi.data_count = 8;
    }

    // Rectangle position
    AppendDataToBuffer(queue, (void *)&pos.x, f32);
    AppendDataToBuffer(queue, (void *)&pos.y, f32);

    // Rectangle size
    AppendDataToBuffer(queue, (void *)&size.width, f32);
    AppendDataToBuffer(queue, (void *)&size.height, f32);

    // Rectangle Color
    AppendDataToBuffer(queue, (void *)&color.r, u8);
    AppendDataToBuffer(queue, (void *)&color.g, u8);
    AppendDataToBuffer(queue, (void *)&color.b, u8);
    AppendDataToBuffer(queue, (void *)&color.a, u8);

    queue->items[queue->count++] = rqi;
    return 1;
}

// TODO: Figure out what to do with the texture
//       Most likely just the id only
internal PushTexture(render_queue *queue, v2 pos, v2 size, texture *tex)
{
    render_queue_item rqi = {0};
    {
        rqi.type = RQI_TYPE_rect;
        rqi.offset = queue->buffer_used;
        rqi.data_count = 5;
    }

    // position
    AppendDataToBuffer(queue, (void *)&pos.x, f32);
    AppendDataToBuffer(queue, (void *)&pos.y, f32);

    // Rectangle size
    AppendDataToBuffer(queue, (void *)&size.width, f32);
    AppendDataToBuffer(queue, (void *)&size.height, f32);
    queue->items[queue->count++] = rqi;
}

// TODO: Text
internal PushText(render_queu *queue)
{
}

