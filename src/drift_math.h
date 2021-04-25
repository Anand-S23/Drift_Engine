#ifndef DRIFT_MATH_H
#define DRIFT_MATH_H

#include <math.h>

#define PIf 3.1415926535897f
#define ONE_OVER_SQUARE_ROOT_OF_TWO_PI 0.3989422804
#define ONE_OVER_SQUARE_ROOT_OF_TWO_PIf 0.3989422804f
#define EULERS_NUMBER 2.7182818284590452353602874713527
#define EULERS_NUMBERf 2.7182818284590452353602874713527f

typedef union v2 
{
    struct
    {
        float x; 
        float y; 
    }; 

    struct 
    {
        float width; 
        float height;
    };
    
} v2;

typedef union v3 
{
    struct 
    {
        float x;
        float y;
        float z;
    };
    
    struct 
    {
        float r;
        float g;
        float b;
    };
    
    float elements[3];
} v3;

typedef union v4 
{
    struct 
    {
        float x;
        float y;
        union 
        {
            struct 
            {
                float z;
                union
                {
                    float w;
                    float radius;
                };
            };

            struct 
            {
                float width;
                float height;
            };
        };
    };
    
    struct 
    {
        float r;
        float g;
        float b;
        float a;
    };
    
    float elements[4];
} v4;

static v2 V2Init(float x, float y)
{
    v2 v = { x, y };
    return v;
}
#define v2(x, y) V2Init(x, y)

static v3 V3Init(float x, float y, float z)
{
    v3 v = { x, y, z };
    return v; 
}
#define v3(x, y, z) V3Init(x, y, z)

static v4 V4Init(float x, float y, float z, float w)
{
    v4 v = { x, y, z, w }; 
    return v; 
}
#define v4(x, y, z, w) V4Init(x, y, z, w)

static v2 V2Scalar(v2 vec, float s)
{
    vec.x *= s;
    vec.y *= s;

    return s;
}

static v3 V3Scalar(v3 vec, float s)
{
    vec.x *= s;
    vec.y *= s;
    vec.z *= s;

    return s;
}

static v4 V4Scalar(v4 vec, float s)
{
    vec.x *= s;
    vec.y *= s;
    vec.z *= s;
    vec.w *= s;

    return s;
}

static v2 V2Add(v2 vec1, v2 vec2)
{
    vec1.x += vec2.x;
    vec1.y += vec2.y;

    return vec1;
}

static v3 V3Add(v3 vec1, v3 vec2)
{
    vec1.x += vec2.x;
    vec1.y += vec2.y;
    vec1.z += vec2.z;

    return vec1;
}

static v4 V4Add(v4 vec1, v4 vec2)
{
    vec1.x += vec2.x;
    vec1.y += vec2.y;
    vec1.z += vec2.z;
    vec1.w += vec2.w;

    return vec1;
}

static v2 V2Sub(v2 vec1, v2 vec2)
{
    vec1.x -= vec2.x;
    vec1.y -= vec2.y;

    return vec1;
}

static v3 V3Sub(v3 vec1, v3 vec2)
{
    vec1.x -= vec2.x;
    vec1.y -= vec2.y;
    vec1.z -= vec2.z;

    return vec1;
}

static v4 V4Sub(v4 vec1, v4 vec2)
{
    vec1.x -= vec2.x;
    vec1.y -= vec2.y;
    vec1.z -= vec2.z;
    vec1.w -= vec2.w;

    return vec1;
}

static float V2Magnitude(v2 v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

static v2 V2Normalize(v2 v)
{
    float scalar = 1.f / V2Magnitude(v);
    return V2Scalar(v, scalar);
}

static float V3Magnitude(v3 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

static v3 V3Normalize(v3 v)
{
    float scalar = 1.f / V2Magnitude(v);
    return V3Scalar(v, scalar);
}

static float V4Magnitude(v4 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

static v4 V4Normalize(v4 v)
{
    float scalar = 1.f / V2Magnitude(v);
    return V4Scalar(v, scalar);
}

typedef struct matrix4f
{
    float elements[4][4];
} matrix4f;

static matrix4f IdentityMatrix()
{
    matrix4f m = {0};
    {
        m.elements[0][0] = 1.f;
        m.elements[1][1] = 1.f;
        m.elements[2][2] = 1.f;
        m.elements[3][3] = 1.f;
    }

    return m;
}

static matrix4f Matrix4fFromVectors(v4 vec1, v4 vec2, v4 vec3, v4 vec4)
{
    matrix4f m = {0};
    {
        m.elements[0][0] = vec1.x;
        m.elements[0][1] = vec2.x;
        m.elements[0][2] = vec3.x;
        m.elements[0][3] = vec4.x;

        m.elements[1][0] = vec1.y;
        m.elements[1][1] = vec2.y;
        m.elements[1][2] = vec3.y;
        m.elements[1][3] = vec4.y;

        m.elements[2][0] = vec1.z;
        m.elements[2][1] = vec2.z;
        m.elements[2][2] = vec3.z;
        m.elements[2][3] = vec4.z;

        m.elements[3][0] = vec1.w;
        m.elements[3][1] = vec2.w;
        m.elements[3][2] = vec3.w;
        m.elements[3][3] = vec4.w;
    }

    return m;
}

// TODO: Matrix Transpose, Inverse
// TODO: Matrix Addition, Subtraction, Multiply

static matrix4f OrthographicMatrix(f32 left, f32 right,
                                   f32 bottom, f32 top,
                                   f32 near_plane, f32 far_plane)
{
    matrix4f m = {0};
    {
        m.elements[0][0] = 2.f / (right - left);
        m.elements[1][1] = 2.f / (top - bottom);
        m.elements[2][2] = -2.f / (far_plane - near_plane);
        m.elements[3][3] = 1.f;
        m.elements[3][0] = (left + right) / (left - right);
        m.elements[3][1] = (bottom + top) / (bottom - top);
        m.elements[3][2] = (far_plane + near_plane) / (near_plane - far_plane);
    }

    return m;
}

/*
static void TranslateMatrix(matrix4f *m, v3 translation)
{
    m->elements[0][3] = translation.x;
    m->elements[1][3] = translation.y;
    m->elements[2][3] = translation.z;
}

static void ScaleMatrix(matrix4f *m, v3 scale)
{
    m->elements[0][0] = scale.x;
    m->elements[1][1] = scale.y;
    m->elements[2][2] = scale.z;
}
*/

#endif
