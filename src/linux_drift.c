#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <x86intrin.h>

#include "drift_platform.h"

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
