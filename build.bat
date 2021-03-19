@echo off

set stb=..\..\stb
set sdl=..\..\SDL2
set src=..\src
set gl=..\..\glext
set gl_lib=..\..\glext\lib\glext.lib

set compiler_options= -DDRIFT_SLOW=1 /nologo /FC /Zi /MD
set linker_options= user32.lib gdi32.lib Shell32.lib winmm.lib opengl32.lib 
set external= -I%src% -I%stb% -I%sdl%\include -I%gl% %sdl%\lib\solo_x64\SDL2.lib %sdl%\lib\solo_x64\SDL2main.lib

set build_file=../src/win32_drift.c

IF NOT EXIST build mkdir build
pushd build
cl %compiler_options% %build_file% %external% /link %linker_options% -subsystem:WINDOWS /out:drift.exe
popd

