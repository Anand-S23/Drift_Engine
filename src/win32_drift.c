#include <windows.h> 
#include <xinput.h>
#include <stdio.h>

#include <gl/gl.h>
#include <glext.h>
#include <wglext.h>

#include "drift_platform.h"
#include "drift_math.h"
#include "app.c"

global platform Global_Platform = {0};
global WINDOWPLACEMENT Global_WindowPosition = { sizeof(Global_WindowPosition) };
global HDC Device_Context;
global HGLRC Render_Context;

// OpenGL
internal void *Win32LoadOpenGLProcedure(const char *name)
{
    void *proc = (void *)wglGetProcAddress(name);

    if (!proc || proc == (void *)0x1 || proc == (void *)0x2 ||
        proc == (void *)0x3 || proc == (void *)-1)
    {
        proc = 0;
    }

    return proc;
}

internal b32 Win32InitOpenGL(HDC *device_context, HINSTANCE instance)
{
    b32 result = 0;
    
    PIXELFORMATDESCRIPTOR pfd = {0};
    {
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
        pfd.cColorBits = 32;
        pfd.cAlphaBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;
    }

    int format_index = ChoosePixelFormat(*device_context, &pfd);

    PIXELFORMATDESCRIPTOR pixel_format;
    DescribePixelFormat(*device_context, format_index,
                        sizeof(PIXELFORMATDESCRIPTOR), &pixel_format);

    SetPixelFormat(*device_context, format_index, &pixel_format);

    HGLRC gl_rc = wglCreateContext(*device_context);
    if (wglMakeCurrent(*device_context, gl_rc))
    {
        PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB =
            (PFNWGLCHOOSEPIXELFORMATARBPROC)Win32LoadOpenGLProcedure("wglChoosePixelFormatARB");
        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
            (PFNWGLCREATECONTEXTATTRIBSARBPROC)Win32LoadOpenGLProcedure("wglCreateContextAttribsARB");

        PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT =
            (PFNWGLSWAPINTERVALEXTPROC)Win32LoadOpenGLProcedure("wglSwapIntervalEXT");

        int attribs[] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB, 32,
            WGL_DEPTH_BITS_ARB, 24,
            WGL_STENCIL_BITS_ARB, 8,
            0
        };

        UINT num_formats = 0;
        wglChoosePixelFormatARB(*device_context, attribs, 0, 1,
                                &format_index, &num_formats);
        
        if (format_index)
        {
            const int context_attribs[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                0
            };
            
            Render_Context = wglCreateContextAttribsARB(*device_context, gl_rc, context_attribs);
            if (Render_Context)
            {
                wglMakeCurrent(*device_context, 0);
                wglDeleteContext(gl_rc);
                wglMakeCurrent(*device_context, Render_Context);
                wglSwapIntervalEXT(1); // vsync off by default
                result = 1;
            }
        }
    }
    
    return result;
}

internal void Win32CleanUpOpenGL(HDC *device_context)
{
    wglMakeCurrent(*device_context, 0);
    wglDeleteContext(Render_Context);
}

internal void Win32SwapBuffers(void)
{
    wglSwapLayerBuffers(Device_Context, WGL_SWAP_MAIN_PLANE);
}

internal void ToggleVSync()
{
    wglSwapIntervalExt(Global_Platform.v_sync);
}
        
// Files
internal void Win32FreeFileMemory(void *memory)
{
    VirtualFree(memory, 0, MEM_RELEASE);
}

internal read_file_result Win32ReadFile(char *filename)
{
    read_file_result result = {0};

    HANDLE file_handle = CreateFileA(filename, GENERIC_READ, 0, 0,
                                     OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (file_handle != INVALID_HANDLE_VALUE) 
    {
        LARGE_INTEGER file_size;
        if (GetFileSizeEx(file_handle, &file_size)) 
        {
            u32 file_size32 = SafeTruncateUInt64(file_size.QuadPart);
            result.memory = VirtualAlloc(0, file_size32, 
                                         MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            if (result.memory) 
            {
                DWORD bytes_read;
                if (ReadFile(file_handle, result.memory, file_size32, &bytes_read, 0) && 
                    bytes_read == file_size32) 
                {
                    result.size = file_size32;
                } 
                else 
                {
                    // TODO: logging
                    Win32FreeFileMemory(result.memory);
                    result.memory = 0;
                }
            } 
            else 
            {
                // TODO: logging
            }
        } 
        else 
        {
            // TODO: logging
        }

        CloseHandle(file_handle);
    } 
    else
    {
        // TODO: logging
    }

    return result;
}

internal b32 Win32WriteFile(char *filename, u32 memory_size, void *memory)
{
    b32 result = 0;

    HANDLE file_handle = CreateFileA(filename, GENERIC_WRITE, 0, 0,
                                     CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

    if (file_handle != INVALID_HANDLE_VALUE) 
    {
        DWORD bytes_written;
        if (WriteFile(file_handle, memory, memory_size, &bytes_written, 0))
        {
            result = (bytes_written == memory_size);
        } 
        else 
        {
            // TODO: logging
        }

        CloseHandle(file_handle);
    } 
    else
    {
        // TODO: logging
    }

    return result;
}

// Core
internal v2 GetWindowDimension(HWND window)
{
    v2 result = {0};

    RECT client_rect;
    GetClientRect(window, &client_rect);
    result.x = client_rect.right - client_rect.left;
    result.y = client_rect.bottom - client_rect.top;

    return result;
}

internal void ToggleFullscreen(HWND window)
{
    DWORD style = GetWindowLongA(window, GWL_STYLE);
    if (style & WS_OVERLAPPEDWINDOW)
    {
        MONITORINFO monitor_info = { sizeof(monitor_info) };
        if (GetWindowPlacement(window, &Global_WindowPosition) &&
            GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY),
                           &monitor_info))
        {
            SetWindowLongA(window, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(window, HWND_TOP, 
                         monitor_info.rcMonitor.left, monitor_info.rcMonitor.top,
                         monitor_info.rcMonitor.right - monitor_info.rcMonitor.left, 
                         monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top, 
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
        else 
        {
            SetWindowLongA(window, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
            SetWindowPlacement(window, &Global_WindowPosition);
            SetWindowPos(window,NULL, 0, 0, 0, 0,
                         SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }
}

LRESULT CALLBACK Win32WindowProc(HWND window, UINT message, 
                                 WPARAM w_param, LPARAM l_param)
{
    LRESULT result = 0; 

    switch (message)
    {
        case WM_CLOSE: 
        case WM_DESTROY: 
        case WM_QUIT:
        {
            Global_Platform.running = 0; 
        } break;

        case WM_LBUTTONDOWN:
        {
            Global_Platform.left_mouse_down = 1;
        } break;

        case WM_LBUTTONUP:
        {
            Global_Platform.left_mouse_down = 0;
        } break;

        case WM_RBUTTONDOWN:
        {
            Global_Platform.right_mouse_down = 1;
        } break;

        case WM_RBUTTONUP:
        {
            Global_Platform.right_mouse_down = 0;
        } break;

        case WM_MBUTTONDOWN:
        {
            Global_Platform.middle_mouse_down = 1;
        } break;

        case WM_MBUTTONUP:
        {
            Global_Platform.middle_mouse_down = 0;
        } break;

        case WM_MOUSEMOVE:
        {
            POINT mouse_pos;
            GetCursorPos(&mouse_pos);
            ScreenToClient(window, &mouse_pos);
            Global_Platform.mouse_x = (f32)mouse_pos.x;
            Global_Platform.mouse_y = (f32)mouse_pos.y;
        } break;

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            u32 vk_code  = (u32)w_param;
            b32 was_down = ((l_param & (1 << 30)) != 0);
            b32 is_down  = ((l_param & (1 << 31)) == 0);

            u32 key_index = 0;

            if (is_down != was_down)
            {
                if (vk_code >= 'A' && vk_code <= 'Z')
                {
                    key_index = KEY_a + (vk_code - 'A');
                }
                else if (vk_code >= '0' && vk_code <= '9')
                {
                    key_index = KEY_0 + (vk_code - '0');
                }
                else
                {
                    if (vk_code == VK_ESCAPE)
                    {
                        key_index = KEY_esc;
                    }
                    else if (vk_code >= VK_F1 && vk_code <= VK_F12)
                    {
                        key_index = KEY_f1 + vk_code - VK_F1;
                    }
                    else if (vk_code == VK_BACK)
                    {
                        key_index = KEY_backspace;
                    }
                    else if (vk_code == VK_DELETE)
                    {
                        key_index = KEY_delete;
                    }
                    else if (vk_code == VK_TAB)
                    {
                        key_index = KEY_tab;
                    }
                    else if (vk_code == VK_SPACE)
                    {
                        key_index = KEY_space;
                    }
                    else if (vk_code == VK_RETURN)
                    {
                        key_index = KEY_enter;
                    }
                    else if (vk_code == VK_CONTROL)
                    {
                        key_index = KEY_ctrl;
                    }
                    else if (vk_code == VK_SHIFT)
                    {
                        key_index = KEY_shift;
                    }
                    else if (vk_code == VK_MENU)
                    {
                        key_index = KEY_alt;
                    }
                    else if (vk_code == VK_UP)
                    {
                        key_index = KEY_up;
                    }
                    else if (vk_code == VK_LEFT)
                    {
                        key_index = KEY_left;
                    }
                    else if (vk_code == VK_DOWN)
                    {
                        key_index = KEY_down;
                    }
                    else if (vk_code == VK_RIGHT)
                    {
                        key_index = KEY_right;
                    }
                }

                Global_Platform.key_down[key_index] = is_down;
            }

            b32 alt_key_was_down = ((l_param & (1 << 29)) != 0);
            if ((vk_code == VK_F4) && alt_key_was_down)
            {
                Global_Platform.running = 0;
            }
            if ((vk_code == VK_RETURN) && alt_key_was_down)
            {
                if (window)
                {
                    ToggleFullscreen(window);
                }
            }
        } break;

        default: 
        {
            result = DefWindowProcA(window, message, w_param, l_param);
        } break;
    }

    return result; 
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, 
                   LPSTR cmd_line, int cmd_show)
{
    WNDCLASSA window_class = {0};
    {
        window_class.style = CS_HREDRAW | CS_VREDRAW;
        window_class.lpfnWndProc = Win32WindowProc;
        window_class.hInstance = instance;
        window_class.lpszClassName = "DriftWindowClass";
        window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    }

    if (RegisterClassA(&window_class))
    {
        drift_application app = DriftInit();

        HWND window = CreateWindowExA(0, window_class.lpszClassName, app.name,
                                      WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
                                      CW_USEDEFAULT, CW_USEDEFAULT, 
                                      app.window_width, app.window_height,
                                      0, 0, instance, 0);

        if (window)
        {
            Device_Context = GetDC(window);
            Win32InitOpenGL(&Device_Context, instance);

            ShowWindow(window, cmd_show);
            UpdateWindow(window);

            v2 window_size = GetWindowDimension(window);

            // Initalize Platform
            {
                Global_Platform.running = 1;
                Global_Platform.fullscreen = app.fullscreen;
                Global_Platform.v_sync = app.v_sync;
                Global_Platform.window_width = window_size.x;
                Global_Platform.window_height = window_size.y;

                Global_Platform.storage_size = Megabytes(64); 
                Global_Platform.transient_storage_size = Megabytes(100);
                Global_Platform.storage =
                    VirtualAlloc(0, Global_Platform.storage_size,
                                 MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
                Global_Platform.transient_storage =
                    VirtualAlloc(0, Global_Platform.transient_storage_size,
                                 MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

                Global_Platform.SwapBuffers = Win32SwapBuffers;
                Global_Platform.LoadOpenGLProcedure = Win32LoadOpenGLProcedure;
                Global_Platform.ReadFile = Win32ReadFile;
                Global_Platform.FreeFileMemory = Win32FreeFileMemory;
                Global_Platform.WriteFile = Win32WriteFile;
            }

            if (Global_Platform.fullscreen)
            {
                ToggleFullscreen(window);
            }

            if (Global_Platform.v_sync)
            {
            // ToggleVSync();
            }

            while (Global_Platform.running)
            {
                MSG message;
                while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }

                Update(&Global_Platform);
            }

            ShowWindow(window, SW_HIDE);
            ReleaseDC(window, Device_Context);
            Win32CleanUpOpenGL(&Device_Context);
        }
        else
        {
            // TODO: Logging
        }
    }
    else 
    {
        // TODO: Logging
    }

    return 0; 
}
