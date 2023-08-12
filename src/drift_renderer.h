#ifndef DRIFT_RENDERER_H
#define DRIFT_RENDERER_H

const char *vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *frag_shader_source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";


float vertices[] = {
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
};

u32 indices[] = {
    0, 1, 3,
    1, 2, 3
};

typedef struct drift_renderer
{
    u32 shader_program;
    u32 vbo;
    u32 vao;
    u32 ebo;
} drift_renderer_t;

#endif
