#ifndef DRIFT_RENDERER_H
#define DRIFT_RENDERER_H

/*
global const char *default_vertex_shader = "#version 330 core\n"
    "layout (location = 0) in vec3 pos;\n"
    "layout (location = 1) in vec3 color;\n"

    "uniform mat4 projection;\n"
    "uniform mat4 view;\n"

    "out f_color;\n"

    "void main()\n"
    "{\n"
        "f_color = color;\n"
        "gl_Position = projection * vec4(pos, 1.0);\n"
   "}\0";

global const char *default_fragment_shader = "#version 330 core\n"
    "out vec4 out_color;\n"

    "in vec4 f_color;\n"

    "void main() {\n"
        "out_color = f_color;\n"
    "}\n\0";
*/

global const char *default_vertex_shader = "#version 330 core\n"
    "layout (location = 0) in vec2 pos;\n"
    "layout (location = 1) in vec4 color;\n"

    "uniform mat4 projection;\n"

    "out vec4 f_color;\n"

    "void main()\n"
    "{\n"
        "gl_Position = projection * vec4(pos.x, pos.y, 0.f, 1.f);\n"
        "f_color = color;\n"
   "}\0";

global const char *default_fragment_shader = "#version 330 core\n"
    "in vec4 f_color;\n"
    "out vec4 frag_color;\n"

    "void main() {\n"
        "frag_color = f_color;\n"
    "}\n\0";

typedef unsigned int shader;

#define MAX_RENDER_OBJECTS 1000

typedef enum render_type
{
    RENDER_TYPE_triangle,
    RENDER_TYPE_rect,
    RENDER_TYPE_texture
} render_type;

// TODO: More generic
typedef struct render_object
{
    render_type type;
    v2 position;
    v2 size;
    v4 color;

    float vertices[36];
} render_object;

typedef struct renderer
{
    int window_width;
    int window_height;
    matrix4f projection_matrix;
    
    shader shader;
    unsigned int vao;
    unsigned int vbo;

    render_object render_list[MAX_RENDER_OBJECTS];
    u32 render_list_count;

    float vertices[6];
} renderer;

#endif
