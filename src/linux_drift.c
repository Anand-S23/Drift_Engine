#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <x86intrin.h>
#define _GNU_SOURCE
#include <dlfcn.h>

#include "drift_platform.h"

typedef struct platform_app_code
{
    init_app_t *init;
    update_app_t *update;
    drift_main_t *drift_main;
    void *dll;
    b32 is_valid;
} platform_app_code_t;

typedef struct {
    const char *dli_fname;  /* Pathname of shared object that
                               contains address */
    void       *dli_fbase;  /* Address at which shared object
                               is loaded */
    const char *dli_sname;  /* Name of nearest symbol with address
                               lower than addr */
    void       *dli_saddr;  /* Exact address of symbol named
                               in dli_sname */
} Dl_info;

static char *drift_platform_get_dll_path(void)
{
    Dl_info info;
    dladdr(drift_platform_get_dll_path, &info);

    int exe_dir_size = strlen(info.dli_fname);
    char *exe_dir = (char *)malloc(sizeof(char) * exe_dir_size);
    memcpy(exe_dir, info.dli_fname, exe_dir_size);

    int last_slash_pos = 0;
    for (int i = 0; exe_dir[i]; ++i)
    {
        if (exe_dir[i] == '/')
        {
            last_slash_pos = i;
        }
    }

    const char *dll_name = "drift_app.dll";
    int dll_name_size = strlen(dll_name);

    int dll_path_size = last_slash_pos + dll_name_size + 1;
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
        dlclose(app_code->dll);
    }

    app_code->dll = 0;
    app_code->is_valid = 0;

    app_code->init = init_app_stub;
    app_code->update = update_app_stub;
    app_code->drift_main = drift_main_stub;
}

static platform_app_code_t drift_platform_load_app_code(char *dll_path)
{
    platform_app_code_t app_code = {0};

    // TODO: Figure out the best mode for dlopen
    app_code.dll = dlopen(dll_path, RTLD_LOCAL | RTLD_LAZY);
    if (!app_code.dll)
    {
        return app_code;
    }

    app_code.init = (init_app_t *)dlsym(app_code.dll, "init_app");
    app_code.update = (update_app_t *)dlsym(app_code.dll, "update_app");
    app_code.drift_main = (drift_main_t *)dlsym(app_code.dll, "drift_main");

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
    free(memory);
}

static read_file_result_t drift_platform_read_file(const char *filename)
{
    read_file_result_t result = {0};

    // TODO: Impelement defer macro for cleaner file handling
    int file_handle = open(filename, O_RDONLY);
    int invalid_value = -1;
    if (file_handle == invalid_value)
    {
        // TODO: Logging
    }

    struct stat file_status;
    if (fstat(file_handle, &file_status) != invalid_value)
    {
        // TODO: Safe truncate for u64 to u32
        u32 file_size = (u32)file_status.st_size;
        result.memory = malloc(result.size);
        if (result.memory)
        {
            result.size = file_size;
            u32 bytes_to_read = file_size;
            u8 *file_data_location = (u8 *)result.memory;
            while (bytes_to_read)
            {
                ssize_t bytes_read = read(file_handle, file_data_location, bytes_to_read);
                if (bytes_read < 0)
                {
                    free(result.memory);
                    result.memory = 0;
                    result.size = 0;
                    return result;
                }

                bytes_to_read -= bytes_read;
                file_data_location += bytes_read;
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

    close(file_handle);
    return result;
}

static b32 drift_platform_write_file(const char *filename, u32 memory_size, void *memory)
{
    int write_file_flags = O_WRONLY | O_CREAT;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int file_handle = open(filename, write_file_flags, mode);

    if (!file_handle)
    {
        return 0;
    }

    u32 bytes_to_write = memory_size;
    u8 *data_location = (u8 *)memory;
    while (bytes_to_write)
    {
        ssize_t bytes_written = write(file_handle, data_location, bytes_to_write);
        if (bytes_written < 0)
        {
            close(file_handle);
            return 0;
        }

        bytes_to_write -= bytes_written;
        data_location += bytes_written;
    }

    close(file_handle);
    return 1;
}
