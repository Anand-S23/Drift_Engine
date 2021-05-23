#ifndef DRIFT_PLATFORM_H
#define DRIFT_PLATFORM_H

#define internal static 
#define local_persist static
#define global static 

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
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

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#include "drift_math.h"
#include "drift_renderer.c"

// TODO: Memset

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

enum
{
#define Key(name, str) KEY_##name,
#include "drift_key_list.h"
    KEY_MAX
};

enum
{
#define Button(name, str) BUTTON_##name,
    Button(up, "Up")
    Button(down, "Down")
    Button(left, "Left")
    Button(right, "Right")

    Button(a, "A")
    Button(b, "B")
    Button(x, "X")
    Button(y, "Y")

    Button(left_shoulder, "Left Shoulder")
    Button(right_shoulder, "Right Shoulder")
    Button(start, "Start")
    Button(back, "Back")
#undef Button
    BUTTON_MAX
};

typedef struct button_state
{
    b32 down;
    b32 release;
} button_state;

typedef struct controller_input
{
    b32 is_connected;
    f32 stick_average_x;
    f32 stick_average_y;
    button_state buttons[BUTTON_MAX];
} controller_input;

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
    b32 key_release[KEY_MAX];

    f32 mouse_x;
    f32 mouse_y;
    b32 left_mouse_down;
    b32 right_mouse_down;
    b32 middle_mouse_down;
    i16 wheel_delta;

    controller_input controller;

    // Functions
    void (*SwapBuffers)(void); 
    void *(*LoadOpenGLProcedure)(char *name);
    read_file_result (*ReadFile)(char *filename);
    void (*FreeFileMemory)(void *memory);
    void (*WriteFile)(char *filename, u32 memory_size, void *memory);
    void (*Log)(int type, char *file, int line, char *format, ...);
} drift_platform;

global drift_platform *platform;

enum
{
    LOG = 0,
    LOG_WARNING = (1 << 0),
    LOG_ERROR = (1 << 1)
};

#define DriftLog(...) platform->Log(LOG, __FILENAME__, __LINE__, __VA_ARGS__)
#define DriftLogWarning(...) platform->Log(LOG_WARNING, __FILENAME__, __LINE__, __VA_ARGS__)
#define DriftLogError(...) platform->Log(LOG_ERROR, __FILENAME__, __LINE__, __VA_ARGS__)

// TODO: Window styles are for non windows platform 
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
} drift_application;

#define INIT_APP(name) void name(void)
typedef INIT_APP(init_app);
INIT_APP(InitAppStub){}

#define UPDATE_APP(name) void name(void)
typedef UPDATE_APP(update_app);
UPDATE_APP(UpdateAppStub){}

#define DRIFT_MAIN(name) drift_application name(drift_platform *platform_)
typedef DRIFT_MAIN(drift_main);
DRIFT_MAIN(DriftMainStub)
{
    drift_application app = {0};
    return app;
}

#endif
