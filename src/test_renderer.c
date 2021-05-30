#include <glad/glad.h>
#include "drift_renderer.h"

internal void InitBuffer(texture_buffer *buffer, int width, int height)
{
    buffer->width = width;
    buffer->height = height;
    buffer->pitch = buffer->width * 4;

    int memory_size = buffer->pitch * buffer->height;
    buffer->memory = malloc(memory_size);
}

internal texture CreateTextureFromBuffer(texture_buffer *buffer)
{
    texture tex;
    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);

    // TODO: Add flags for texture GL options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (buffer->memory)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, buffer->width, buffer->height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, (u8 *)buffer->memory);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        DriftLogWarning("stbi_load could not load texture");
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    return tex;
}

internal void ClearBuffer(texture_buffer *buffer, v4 color)
{
    int width = buffer->width;
    int height = buffer->height;
    
    u8 *row = (u8 *)buffer->memory;
    for (int y = 0; y < height; ++y)
    {
        u32 *pixel = (u32 *)row;
        for (int x = 0; x < width; ++x)
        {
            u8 red   = (u8)(color.r * 255.f);
            u8 green = (u8)(color.g * 255.f);
            u8 blue  = (u8)(color.b * 255.f);
            u8 alpha = (u8)(color.a * 255.f);

            *pixel++ = ((alpha << 24) | (red << 16) | (green << 8) | blue);
        }

        row += buffer->pitch;
    }
}

internal void DrawFilledRect(texture_buffer *buffer, v4 rect, v4 color)
{
    i32 min_x = Max(0, (i32)rect.x);
    i32 min_y = Max(0, (i32)rect.y);
    i32 max_x = Min((buffer->width), (min_x + (i32)rect.width));
    i32 max_y = Min((buffer->height), (min_y + (i32)rect.height));
    
    u8 *row = (u8 *)buffer->memory + min_x * 4 + min_y * buffer->pitch; 

    u8 red   = (u8)(color.r * 255.f);
    u8 green = (u8)(color.g * 255.f);
    u8 blue  = (u8)(color.b * 255.f);
    u8 alpha = (u8)(color.a * 255.f);

    // TODO: Bi-linear blending for non-0 or non-255 alpha values
    if (alpha > 0)
    {
        for(i32 j = min_y; j < max_y; ++j)
        {
            u32 *pixel = (u32 *)row;
            for(i32 i = min_x; i < max_x; ++i)
            {

                *pixel++ = ((alpha << 24) | (red << 16) | (green << 8) | blue);
            }

            row += buffer->pitch; 
        }
    }
}

internal void DrawFilledRectU32(texture_buffer *buffer, 
                                v4 rect, u32 color)
{
    i32 min_x = Max(0, (i32)rect.x);
    i32 min_y = Max(0, (i32)rect.y);
    i32 max_x = Min((buffer->width), (min_x + (i32)rect.width));
    i32 max_y = Min((buffer->height), (min_y + (i32)rect.height));
    
    u8 *row = (u8 *)buffer->memory + min_x * 4 + min_y * buffer->pitch; 

    u8 *color_channels = (u8 *)&color;
    u8 red   = (u8)(color_channels[0]);
    u8 green = (u8)(color_channels[1]);
    u8 blue  = (u8)(color_channels[2]);
    u8 alpha = (u8)(color_channels[3]);
    
    // TODO: Bi-linear blending for non-0 or non-255 alpha values
    if (alpha > 0)
    {
        for(i32 j = min_y; j < max_y; ++j)
        {
            u32 *pixel = (u32 *)row;
            for(i32 i = min_x; i < max_x; ++i)
            {
                *pixel++ = ((alpha << 24) | (red << 16) | (green << 8) | blue);
            }

            row += buffer->pitch; 
        }
    }
}

