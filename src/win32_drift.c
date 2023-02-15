#include <windows.h>

#include "drift_platform.h"

typedef struct platform_app_code
{
    init_app_t *init;
    update_app_t *update;
    drift_main_t *drift_main;
    HMODULE dll;
    b32 is_valid;
} platform_app_code_t;


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

