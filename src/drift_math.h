#ifndef DRIFT_MATH_H
#define DRIFT_MATH_H

#include <math.h>

#define PIf                               3.1415926535897f
#define TAU                               6.28318530718f
#define PI_OVER_TWO                       1.57079632679f
#define ONE_OVER_SQUARE_ROOT_OF_TWO_PI    0.3989422804
#define ONE_OVER_SQUARE_ROOT_OF_TWO_PIf   0.3989422804f
#define EULERS_NUMBERf                    2.7182818284590452353602874713527f
#define DEG_TO_RAD                        0.0174532925f
#define RAD_TO_DEG                        57.2958f

#define bytes(n)     (n)
#define kilobytes(n) (bytes(n)*1024LL)
#define megabytes(n) (kilobytes(n)*1024LL)
#define gigabytes(n) (megabytes(n)*1024LL)
#define terabytes(n) (gigabytes(n)*1024LL)

#define min(a, b)               ((a) < (b) ? (a) : (b))
#define max(a, b)               ((a) > (b) ? (a) : (b))
#define absolute_value(a)       ((a) > 0 ? (a) : -(a))
#define lerp(a, b, t)           (a + (b - a) * t)
#define clamp01(val)            min(1.f, max(0.f, (val)))
#define clamp(low, high, val)   min((high), max((low), (val)))
#define square(a)               ((a) * (a))

// Vector Math

typedef union v2 
{
    struct
    {
        f32 x; 
        f32 y; 
    }; 

    struct 
    {
        f32 width; 
        f32 height;
    };
    
} v2_t;

typedef union v3 
{
    struct 
    {
        f32 x;
        f32 y;
        f32 z;
    };
    
    struct 
    {
        f32 r;
        f32 g;
        f32 b;
    };
    
    f32 elements[3];
} v3_t;

typedef union v4 
{
    struct 
    {
        f32 x;
        f32 y;
        union 
        {
            struct 
            {
                f32 z;
                union
                {
                    f32 w;
                    f32 radius;
                };
            };

            struct 
            {
                f32 width;
                f32 height;
            };
        };
    };
    
    struct 
    {
        f32 r;
        f32 g;
        f32 b;
        f32 a;
    };
    
    f32 elements[4];
} v4_t;

v2_t v2_init(f32 x, f32 y)
{
    v2_t v = { .x = x, .y = y };
    return v;
}
#define v2(x, y) v2_init(x, y)

v3_t v3_init(f32 x, f32 y, f32 z)
{
    v3_t v = { .x = x, .y = y, .z = z };
    return v; 
}
#define v3(x, y, z) v3_init(x, y, z)

v4_t v4_init(f32 x, f32 y, f32 z, f32 w)
{
    v4_t v = { .x = x, .y = y, .z = z, .w = w }; 
    return v; 
}
#define v4(x, y, z, w) v4_init(x, y, z, w)

v2_t v2_scalar(v2_t vec, f32 s)
{
    vec.x *= s;
    vec.y *= s;

    return vec;
}

v3_t v3_scalar(v3_t vec, f32 s)
{
    vec.x *= s;
    vec.y *= s;
    vec.z *= s;

    return vec;
}

v4_t v4_scalar(v4_t vec, f32 s)
{
    vec.x *= s;
    vec.y *= s;
    vec.z *= s;
    vec.w *= s;

    return vec;
}

v2_t v2_add(v2_t vec1, v2_t vec2)
{
    vec1.x += vec2.x;
    vec1.y += vec2.y;

    return vec1;
}

v3_t v3_add(v3_t vec1, v3_t vec2)
{
    vec1.x += vec2.x;
    vec1.y += vec2.y;
    vec1.z += vec2.z;

    return vec1;
}

v4_t v4_add(v4_t vec1, v4_t vec2)
{
    vec1.x += vec2.x;
    vec1.y += vec2.y;
    vec1.z += vec2.z;
    vec1.w += vec2.w;

    return vec1;
}

v2_t v2_subtract(v2_t vec1, v2_t vec2)
{
    vec1.x -= vec2.x;
    vec1.y -= vec2.y;

    return vec1;
}

v3_t v3_subtract(v3_t vec1, v3_t vec2)
{
    vec1.x -= vec2.x;
    vec1.y -= vec2.y;
    vec1.z -= vec2.z;

    return vec1;
}

v4_t v4_subtract(v4_t vec1, v4_t vec2)
{
    vec1.x -= vec2.x;
    vec1.y -= vec2.y;
    vec1.z -= vec2.z;
    vec1.w -= vec2.w;

    return vec1;
}

f32 v2_magnitude(v2_t vec)
{
    return (f32)sqrt(vec.x * vec.x + vec.y * vec.y);
}

v2_t v2_normalize(v2_t vec)
{
    f32 scalar = 1.f / v2_magnitude(vec);
    return v2_scalar(vec, scalar);
}

f32 v3_magnitude(v3_t vec)
{
    return (f32)sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

v3_t v3_normalize(v3_t vec)
{
    f32 scalar = 1.f / v3_magnitude(vec);
    return v3_scalar(vec, scalar);
}

f32 v4_magnitude(v4_t vec)
{
    return (f32)sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
}

v4_t v4_normalize(v4_t vec)
{
    f32 scalar = 1.f / v4_magnitude(vec);
    return v4_scalar(vec, scalar);
}

f32 v2_dot(v2_t vec1, v2_t vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y; 
}

f32 v3_dot(v3_t vec1, v3_t vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z; 
}

f32 v4_dot(v4_t vec1, v4_t vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w;
}

v3_t v3_cross(v3_t vec1, v3_t vec2)
{
    return v3(vec1.y * vec2.z - vec1.z * vec2.y,
              vec1.z * vec2.x - vec1.x * vec2.z,
              vec1.x * vec2.y - vec1.y * vec2.x);
}

u32 PackRGBA(v4_t color)
{
    u8 red   = (u8)(color.r * 255.f);
    u8 green = (u8)(color.g * 255.f);
    u8 blue  = (u8)(color.b * 255.f);
    u8 alpha = (u8)(color.a * 255.f);

    u32 result = ((alpha << 24) | (blue << 16) | (green << 8) | red);
    return result;
}

v4_t UnpackRGBA(u32 color)
{
    u8 red   = ((u8)color);
    u8 green = (color >> 8);
    u8 blue  = (color >> 16);
    u8 alpha = (color >> 24);

    return v4_scalar(v4(red, green, blue, alpha), (1.f / 255.f));
}

typedef struct matrix4f
{
    f32 elements[4][4];
} matrix4f_t;

matrix4f_t identity_matrix(void)
{
    matrix4f_t m = {0};
    {
        m.elements[0][0] = 1.f;
        m.elements[1][1] = 1.f;
        m.elements[2][2] = 1.f;
        m.elements[3][3] = 1.f;
    }

    return m;
}

matrix4f_t matrix4f(f32 e00, f32 e01, f32 e02, f32 e03,
                           f32 e10, f32 e11, f32 e12, f32 e13,
                           f32 e20, f32 e21, f32 e22, f32 e23,
                           f32 e30, f32 e31, f32 e32, f32 e33)
{
    matrix4f_t m = {0};
    {
        m.elements[0][0] = e00;
        m.elements[0][1] = e01;
        m.elements[0][2] = e02;
        m.elements[0][3] = e03;

        m.elements[1][0] = e10;
        m.elements[1][1] = e11;
        m.elements[1][2] = e12;
        m.elements[1][3] = e13;

        m.elements[2][0] = e20;
        m.elements[2][1] = e21;
        m.elements[2][2] = e22;
        m.elements[2][3] = e23;

        m.elements[3][0] = e30;
        m.elements[3][1] = e31;
        m.elements[3][2] = e32;
        m.elements[3][3] = e33;
    }

    return m;
}

matrix4f_t matrix4f_from_v4(v4_t vec1, v4_t vec2, v4_t vec3, v4_t vec4)
{
    matrix4f_t m = {0};
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

matrix4f_t matrix4f_transpose(matrix4f_t mat)
{
    matrix4f_t m = {0};
    for (int j = 0; j < 4; ++j)
    {
        for (int i = 0; i < 4; ++i)
        {
            m.elements[j][i] = mat.elements[i][j];
        }
    }

    return m;
}

matrix4f_t matrix4f_add(matrix4f_t m1, matrix4f_t m2)
{
    for (int j = 0; j < 4; ++j)
    {
        for (int i = 0; i < 4; ++i)
        {
            m1.elements[j][i] += m2.elements[j][i];
        }
    }

    return m1;
}

matrix4f_t matrix4f_subtract(matrix4f_t m1, matrix4f_t m2)
{
    for (int j = 0; j < 4; ++j)
    {
        for (int i = 0; i < 4; ++i)
        {
            m1.elements[j][i] -= m2.elements[j][i];
        }
    }

    return m1;
}

matrix4f_t matrix4f_scalar(matrix4f_t m, f32 s)
{

    for (int j = 0; j < 4; ++j)
    {
        for (int i = 0; i < 4; ++i)
        {
            m.elements[j][i] *= s;
        }
    }

    return m;
}

v4_t matrix4f_vec_multiply(matrix4f_t m, v4_t v)
{
    return v4(m.elements[0][0] * v.x + m.elements[0][1] * v.y + 
              m.elements[0][2] * v.z + m.elements[0][3] * v.w,

              m.elements[1][0] * v.x + m.elements[1][1] * v.y + 
              m.elements[1][2] * v.z + m.elements[1][3] * v.w,

              m.elements[2][0] * v.x + m.elements[2][1] * v.y + 
              m.elements[2][2] * v.z + m.elements[2][3] * v.w,

              m.elements[3][0] * v.x + m.elements[3][1] * v.y + 
              m.elements[3][2] * v.z + m.elements[3][3] * v.w);
}

matrix4f_t matrix4f_multiply(matrix4f_t m1, matrix4f_t m2)
{
    f32 e00 = m1.elements[0][0] * m2.elements[0][0] +
              m1.elements[0][1] * m2.elements[1][0] +
              m1.elements[0][2] * m2.elements[2][0] +
              m1.elements[0][3] * m2.elements[3][0];

    f32 e01 = m1.elements[0][0] * m2.elements[0][1] +
              m1.elements[0][1] * m2.elements[1][1] +
              m1.elements[0][2] * m2.elements[2][1] +
              m1.elements[0][3] * m2.elements[3][1];

    f32 e02 = m1.elements[0][0] * m2.elements[0][2] +
              m1.elements[0][1] * m2.elements[1][2] +
              m1.elements[0][2] * m2.elements[2][2] +
              m1.elements[0][3] * m2.elements[3][2];

    f32 e03 = m1.elements[0][0] * m2.elements[0][3] +
              m1.elements[0][1] * m2.elements[1][3] +
              m1.elements[0][2] * m2.elements[2][3] +
              m1.elements[0][3] * m2.elements[3][3];

    f32 e10 = m1.elements[1][0] * m2.elements[0][0] +
              m1.elements[1][1] * m2.elements[1][0] +
              m1.elements[1][2] * m2.elements[2][0] +
              m1.elements[1][3] * m2.elements[3][0];

    f32 e11 = m1.elements[1][0] * m2.elements[0][1] +
              m1.elements[1][1] * m2.elements[1][1] +
              m1.elements[1][2] * m2.elements[2][1] +
              m1.elements[1][3] * m2.elements[3][1];

    f32 e12 = m1.elements[1][0] * m2.elements[0][2] +
              m1.elements[1][1] * m2.elements[1][2] +
              m1.elements[1][2] * m2.elements[2][2] +
              m1.elements[1][3] * m2.elements[3][2];

    f32 e13 = m1.elements[1][0] * m2.elements[0][3] +
              m1.elements[1][1] * m2.elements[1][3] +
              m1.elements[1][2] * m2.elements[2][3] +
              m1.elements[1][3] * m2.elements[3][3];

    f32 e20 = m1.elements[2][0] * m2.elements[0][0] +
              m1.elements[2][1] * m2.elements[1][0] +
              m1.elements[2][2] * m2.elements[2][0] +
              m1.elements[2][3] * m2.elements[3][0];

    f32 e21 = m1.elements[2][0] * m2.elements[0][1] +
              m1.elements[2][1] * m2.elements[1][1] +
              m1.elements[2][2] * m2.elements[2][1] +
              m1.elements[2][3] * m2.elements[3][1];

    f32 e22 = m1.elements[2][0] * m2.elements[0][2] +
              m1.elements[2][1] * m2.elements[1][2] +
              m1.elements[2][2] * m2.elements[2][2] +
              m1.elements[2][3] * m2.elements[3][2];

    f32 e23 = m1.elements[2][0] * m2.elements[0][3] +
              m1.elements[2][1] * m2.elements[1][3] +
              m1.elements[2][2] * m2.elements[2][3] +
              m1.elements[2][3] * m2.elements[3][3];

    f32 e30 = m1.elements[3][0] * m2.elements[0][0] +
              m1.elements[3][1] * m2.elements[1][0] +
              m1.elements[3][2] * m2.elements[2][0] +
              m1.elements[3][3] * m2.elements[3][0];

    f32 e31 = m1.elements[3][0] * m2.elements[0][1] +
              m1.elements[3][1] * m2.elements[1][1] +
              m1.elements[3][2] * m2.elements[2][1] +
              m1.elements[3][3] * m2.elements[3][1];

    f32 e32 = m1.elements[3][0] * m2.elements[0][2] +
              m1.elements[3][1] * m2.elements[1][2] +
              m1.elements[3][2] * m2.elements[2][2] +
              m1.elements[3][3] * m2.elements[3][2];

    f32 e33 = m1.elements[3][0] * m2.elements[0][3] +
              m1.elements[3][1] * m2.elements[1][3] +
              m1.elements[3][2] * m2.elements[2][3] +
              m1.elements[3][3] * m2.elements[3][3];

    return matrix4f(e00, e01, e02, e03, e10, e11, e12, e13,
                    e20, e21, e22, e23, e30, e31, e32, e33);
}

matrix4f_t matrix4f_inverse(matrix4f_t m)
{
    f32 m00 = m.elements[0][0]; f32 m01 = m.elements[0][1];
    f32 m02 = m.elements[0][2]; f32 m03 = m.elements[0][3];

    f32 m10 = m.elements[1][0]; f32 m11 = m.elements[1][1];
    f32 m12 = m.elements[1][2]; f32 m13 = m.elements[1][3];

    f32 m20 = m.elements[2][0]; f32 m21 = m.elements[2][1];
    f32 m22 = m.elements[2][2]; f32 m23 = m.elements[2][3];

    f32 m30 = m.elements[3][0]; f32 m31 = m.elements[3][1];
    f32 m32 = m.elements[3][2]; f32 m33 = m.elements[3][3];

    matrix4f_t res;
    res.elements[0][0] = m11*m22*m33 - m11*m23*m32 - m21*m12*m33 +
                         m12*m13*m32 + m31*m12*m23 - m31*m13*m22;
    res.elements[0][1] = -m01*m22*m33 + m01*m23*m32 + m21*m02*m33 -
                         m21*m03*m32 - m31*m02*m23 + m31*m03*m22;
    res.elements[0][2] = m01*m12*m33 - m01*m13*m32 - m11*m02*m33 +
                         m11*m03*m32 + m31*m02*m13 - m31*m03*m12;
    res.elements[0][3] = -m01*m12*m23 + m01*m13*m22 + m11*m02*m23 -
                         m11*m03*m22 - m21*m02*m13 + m21*m03*m12;

    res.elements[1][0] = -m10*m22*m33 + m10*m23*m32 + m20*m12*m33 -
                         m20*m13*m32 - m30*m12*m23 + m30*m13*m22;
    res.elements[1][1] = m00*m22*m33 - m00*m23*m32 - m20*m02*m33 +
                         m20*m03*m32 + m30*m02*m23 - m30*m03*m22;
    res.elements[1][2] = -m00*m12*m33 + m00*m13*m32 + m10*m02*m33 -
                         m10*m03*m32 - m30*m02*m13 + m30*m03*m12;
    res.elements[1][3] = m00*m12*m23 - m00*m13*m22 - m10*m02*m23 +
                         m10*m03*m22 + m20*m02*m13 - m20*m03*m12;

    res.elements[2][0] = m10*m21*m33 - m10*m23*m31 - m20*m11*m33 +
                         m20*m13*m31 + m30*m11*m23 - m30*m13*m21;
    res.elements[2][1] = -m00*m21*m33 + m00*m23*m31 + m20*m01*m33 -
                         m20*m03*m31 - m30*m01*m23 + m30*m03*m21;
    res.elements[2][2] = m00*m11*m33 - m00*m13*m31 - m10*m01*m33 +
                         m10*m03*m31 + m30*m01*m13 - m30*m03*m11;
    res.elements[2][3] = m00*m11*m23 + m00*m13*m21 + m10*m01*m23 -
                         m10*m03*m21 - m20*m01*m13 + m20*m03*m11;

    res.elements[3][0] = -m10*m21*m32 + m10*m22*m31 + m20*m11*m32 -
                         m20*m12*m31 - m30*m11*m22 + m30*m12*m21;
    res.elements[3][1] = m00*m21*m32 - m00*m22*m31 - m20*m01*m32 +
                         m20*m02*m31 + m30*m01*m22 - m30*m02*m21;
    res.elements[3][2] = -m00*m11*m32 + m00*m12*m31 + m10*m01*m32 -
                         m10*m02*m31 - m30*m01*m12 + m30*m02*m11;
    res.elements[3][3] = m00*m11*m22 - m00*m12*m21 - m10*m01*m22 +
                         m10*m02*m21 + m20*m01*m12 - m20*m02*m11;

    return res;
}

matrix4f_t orthographic_matrix4f(f32 left, f32 right,
                                        f32 bottom, f32 top,
                                        f32 near_plane, f32 far_plane)
{
    matrix4f_t m = {0};
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
TODO:
void translate_matrix(matrix4f_t *m, v3_t translation);
void scale_matrix(matrix4f_t *m, v3_t scale);
*/

// Qauterions

typedef struct quat
{
    struct
    {
        f32 x;
        f32 y;
        f32 z;
    };

    f32 w;
} quat_t;

// TODO: All quat funcs

#endif
