#ifndef DRIFT_MATH_H
#define DRIFT_MATH_H

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

#endif
