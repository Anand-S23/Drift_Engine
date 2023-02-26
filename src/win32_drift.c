#include <windows.h>

#include "win32_drift.h"

static char *drift_platform_get_dll_path(void)
{
    char exe_path[512];
    DWORD exe_path_size = GetModuleFileNameA(0, exe_path, sizeof(exe_path));

    int exe_dir_size = exe_path_size;
    char *exe_dir = (char *)malloc(sizeof(char) * exe_dir_size);
    memcpy(exe_dir, exe_path, exe_dir_size);

    int last_slash_pos = 0;
    for (int i = 0; exe_dir[i]; ++i)
    {
        if (exe_dir[i] == '\\')
        {
            last_slash_pos = i;
        }
    }

    const char *dll_name = "drift_app.dll";
    int dll_name_size = strlen(dll_name);

    int dll_path_size = last_slash_pos + dll_name_size + 1;
    // TODO: Will have do this a different way
    char *dll_path = (char *)malloc(sizeof(char) * dll_path_size);
    memcpy(dll_path, exe_dir, last_slash_pos + 1);
    memcpy(dll_path + last_slash_pos + 1, dll_name, dll_name_size);

    free(exe_dir);
    return dll_path;
}

static void drift_platform_unload_app_code(platform_app_code_t *app_code)
{
    if (app_code->dll)
    {
        FreeLibrary(app_code->dll);
    }

    app_code->dll = 0;
    app_code->is_valid = 0;

    app_code.init = init_app_stub;
    app_code.update = update_app_stub;
    app_code.drift_main = drift_main_stub;
}

static platform_app_code_t drift_platform_load_app_code(char *dll_path)
{
    platform_app_code_t app_code = {0};

    app_code.dll = LoadLibraryA(dll_path);
    if (!app_code.dll)
    {
        return app_code;
    }
    
    app_code.init = (init_app_t *)GetProcAddress(app_code.dll, "init_app");
    app_code.update = (update_app_t *)GetProcAddress(app_code.dll, "update_app");
    app_code.drift_main = (drift_main_t *)GetProcAddress(app_code.dll, "drift_main");
    
    if (!app_code.init || !app_code.update|| !app_code.drift_main)
    {
        app_code.init = init_app_stub;
        app_code.update = update_app_stub;
        app_code.drift_main = drift_main_stub;
        return app_code;
    }

    app_code.is_valid = 1;
    return app_code;
}

static void drift_platform_free_file_memory(void *memory)
{
    VirtualFree(memory, 0, MEM_RELEASE);
}

static read_file_result_t drift_platform_read_file(const char *filename)
{
    read_file_result_t result = {0};

    // TODO: Impelement defer macro for cleaner file handling
    HANDLE file_handle = CreateFileA(filename, GENERIC_READ, 0, 0,
                                     OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (file_handle == INVALID_HANDLE_VALUE) 
    {
        // TODO: Logging
        return result;
    }

    LARGE_INTEGER file_size;
    if (GetFileSizeEx(file_handle, &file_size)) 
    {
        // TODO: Safe truncate for u64 to u32
        u32 file_size32 = (u32)file_size.QuadPart;
        result.memory =
            VirtualAlloc(0, file_size32, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

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
                // TODO: Logging
                Win32FreeFileMemory(result.memory);
                result.memory = 0;
                result.size = 0;
            }
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

    CloseHandle(file_handle);
    return result;
}

static b32 drift_platform_write_file(const char *filename, u32 memory_size, void *memory)
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
            // TODO: Logging
        }

        CloseHandle(file_handle);
    } 
    else
    {
        // TODO: Logging
    }

    return result;
}

