#ifndef DRIFT_PLATFORM_H
#define DRIFT_PLATFORM_H

static char *drift_platform_get_dll_path(void);
// static void drift_platform_unload_app_code(platform_app_code_t *app_code);
// static platform_app_code_t drift_platform_load_app_code(char *dll_path);
static void drift_platform_free_file_memory(void *memory);
static read_file_result_t drift_platform_read_file(const char *filename);
static b32 drift_platform_write_file(const char *filename, u32 memory_size, void *memory);

#endif
