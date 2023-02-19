#ifndef LINUX_DRIFT_H
#define LINUX_DRIFT_H

typedef struct platform_app_code
{
    init_app_t *init;
    update_app_t *update;
    drift_main_t *drift_main;
    void *dll;
    b32 is_valid;
} platform_app_code_t;

typedef struct
{
    const char *dli_fname;
    void *dli_fbase;
    const char *dli_sname;
    void *dli_saddr;
} Dl_info;

// App Code
static char *drift_platform_get_dll_path(void);
static void drift_platform_unload_app_code(platform_app_code_t *app_code);
static platform_app_code_t drift_platform_load_app_code(char *dll_path);

// File IO
static void drift_platform_free_file_memory(void *memory);
static read_file_result_t drift_platform_read_file(const char *filename);
static b32 drift_platform_write_file(const char *filename, u32 memory_size, void *memory);

#endif
