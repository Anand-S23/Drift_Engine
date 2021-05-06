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

    "uniform mat4 projection;\n"

    "void main()\n"
    "{\n"
        "gl_Position = projection * vec4(pos.x, pos.y, 0.f, 1.f);\n"
   "}\0";

global const char *default_fragment_shader = "#version 330 core\n"
    "out vec4 frag_color;\n"

    "void main() {\n"
        "frag_color = vec4(1, 0, 0, 1);\n"
    "}\n\0";

#define MAX_RENDER_OBJECTS 1000

typedef struct shader
{
    unsigned int program_id;
} shader;

typedef enum command_type
{
    COMMAND_clear,
    COMMAND_render_point,
    COMMAND_render_line,
    COMMAND_render_triangle,
    COMMAND_render_rect,
    COMMAND_render_texture
} command_type;

typedef struct command_data
{
    command_type type;
    void *data;
    u32 data_size;
    u32 position;
} command_data;

typedef struct command_node
{
    command_data *command;
    struct comment_node *next;
} command_node;

typedef struct command_buffer
{
    command_node *head;
    command_node *tail;
    u32 command_count;
} command_buffer;

typedef struct renderer
{
    int window_width;
    int window_height;
    matrix4f projection_matrix;
    
    shader shader;
    unsigned int vao;
    unsigned int vbo;

    command_buffer cb;
    float vertices[6];
} renderer;

#endif
