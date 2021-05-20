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

internal voidWin32LogInternal(char *type, char *file,
                              int line, char *format, ...);

internal void Win32Log(char *file, int line, char *format, ...);
internal void Win32LogWarning(char *file, int line, char *format, ...);
internal void Win32LogError(char *file, int line, char *format, ...);

#define W32Log(...) Win32Log(__FILENAME__, __LINE__, __VA_ARGS__)
#define W32LogWarning(...) Win32LogWarning(__FILENAME__, __LINE__, __VA_ARGS__)
#define W32LogError(...) Win32LogError(__FILENAME__, __LINE__, __VA_ARGS__)

#endif
