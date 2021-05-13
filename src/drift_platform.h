#ifndef DRIFT_PLATFORM_H
#define DRIFT_PLATFORM_H

#define internal static 
#define local_persist static
#define global static 

#include <stdint.h>

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   b8;
typedef int16_t  b16;
typedef int32_t  b32;
typedef int64_t  b64;
typedef float    f32;
typedef double   f64;

#define Bytes(n)     (n)
#define Kilobytes(n) (Bytes(n)*1024LL)
#define Megabytes(n) (Kilobytes(n)*1024LL)
#define Gigabytes(n) (Megabytes(n)*1024LL)
#define Terabytes(n) (Gigabytes(n)*1024LL)

#define ArraySize(arr) (sizeof(arr) / sizeof((arr)[0]))
#define Min(a, b) ((a) < (b) ? (a) : (b))
#define Max(a, b) ((a) > (b) ? (a) : (b))
#define AbsoluteValue(a) ((a) > 0 ? (a) : -(a))

#if DRIFT_SLOW
#define Assert(expr) if(!(expr)) {*(int *)0 = 0;}
#else 
#define Assert(expr)
#endif

inline u32 SafeTruncateUInt64(u64 value)
{
    Assert(value <= 0xFFFFFFFF);
    return (u32)value;
}

typedef struct read_file_result
{
    u32 size;
    void *memory;
} read_file_result;

enum {
#define Key(name, str) KEY_##name,
#include "drift_key_list.h"
    KEY_MAX
};

typedef struct drift_platform
{
    // Options
    b32 running;
    b32 fullscreen;
    b32 v_sync;
    int window_width;
    int window_height;
    
    // App Memory
    void *storage;
    u64 storage_size; 
    void *transient_storage; 
    u32 transient_storage_size;

    b32 initialized;

    // Time
    f32 current_time;
    f32 last_time;

    // Input
    b32 key_down[KEY_MAX];

    f32 mouse_x;
    f32 mouse_y;
    b32 left_mouse_down;
    b32 right_mouse_down;
    b32 middle_mouse_down;
    i16 wheel_delta;

    // TODO: Gamepad sticks/triggers
    struct
    {
        b32 back;
        b32 start;
        b32 right_shoulder;
        b32 left_shoulder;
        b32 right_thumb;
        b32 left_thumb;

        b32 d_pad_up;
        b32 d_pad_down;
        b32 d_pad_right;
        b32 d_pad_left;

        b32 button_a;
        b32 button_b;
        b32 button_x;
        b32 button_y;
    } gamepad;

    // Functions
    void (*SwapBuffers)(void); 
    void *(*LoadOpenGLProcedure)(char *name);
    read_file_result (*ReadFile)(char *filename);
    void (*FreeFileMemory)(void *memory);
    void (*WriteFile)(char *filename, u32 memory_size, void *memory);
    void (*LogWarning)(char *format);
    void (*LogError)(char *message_format, ...);
} drift_platform;

global drift_platform *platform;

typedef enum drift_window_styles
{
    DWS_border = 0x00800000L,
    DWS_caption = 0x00C00000L,
    DWS_child = 0x40000000L,
    DWS_clipchildren = 0x02000000L,
    DWS_clipsiblings = 0x04000000L,
    DWS_disabled = 0x08000000L,
    DWS_dlgframe = 0x00400000L,
    DWS_group = 0x00020000L,
    DWS_hscroll = 0x00020000L,
    DWS_maximize = 0x01000000L,
    DWS_maximizebox = 0x00010000L,
    DWS_minimize = 0x20000000L,
    DWS_minimizebox = 0x00020000L,
    DWS_overlapped = 0x00000000L,
    DWS_overlappedwindow = (0x00000000L | 0x00C00000L | 0x00080000L |
                            0x00040000L | 0x00020000L | 0x00010000L),
    DWS_popup = 0x80000000L,
    DWS_popupwindow = (0x80000000L | 0x00800000L | 0x00080000L),
    DWS_sizebox = 0x00040000L,
    DWS_sysmenu = 0x00080000L,
    DWS_tabstop = 0x00010000L,
    DWS_visible = 0x10000000L,
    DWS_vscroll = 0x00200000L
} drift_window_styles;

typedef struct drift_application
{
    const char *name;
    int window_width;
    int window_height;
    b32 fullscreen;
    b32 v_sync;

    drift_window_styles window_style;
    b32 window_exact; // TODO: move this to window styles

    void (*Init)();
    void (*Update)();
} drift_application;

#endif
