#ifndef WIN32_DRIFT_H
#define WIN32_DRIFT_H

typedef struct win32_app_code
{
    init_app *Init;
    update_app *Update;
    drift_main *DriftMain;

    HMODULE dll;
    FILETIME dll_last_write_time;
    b32 is_valid;
} win32_app_code;

#endif
