#ifndef DRIFT_RENDERER_H
#define DRIFT_RENDERER_H

const char *default_vertex_shader = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *default_fragment_shader = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

#define MAX_RENDER_OBJECTS 1000

#define GLProc(name, type) PFNGL##type##PROC gl##name;
#include "drift_gl_proc_list.h"

internal void LoadAllOpenGLProcedures(platform *platform)
{
#define GLProc(name, type) gl##name = (PFNGL##type##PROC)platform->LoadOpenGLProcedure("gl" #name);
#include "drift_gl_proc_list.h"
}

typedef struct shader
{
    unsigned int program_id;
} shader;

typedef enum render_type
{
    RENDER_TYPE_triangle;
    RENDER_TYPE_rect;
    RENDER_TYPE_texture;
} render_type;

typedef struct render_object
{
    render_type type;
    v2 position;
    v4 color;
} render_object;

typedef struct renderer
{
    int window_width;
    int window_height;
    matrix4f project_matrix;
    
    unsigned int vao;
    unsigned int vbo;
    shader shader;

    render_object render_list[MAX_RENDER_OBJECTS];
    int render_list_count;
} renderer;

#endif
