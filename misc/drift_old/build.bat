@echo off

set stb=..\..\stb
set g=..\..\glad
set gl=..\..\..\glext
set src=..\src


set compiler_options= -DDRIFT_SLOW=1 /nologo /FC /Zi /MD
set linker_options= user32.lib gdi32.lib Shell32.lib winmm.lib opengl32.lib kernel32.lib
set external= %g%\src\glad.c -I%src% -I%stb% -I%g%\include -I%gl%

set build_file=../src/win32_drift.c

IF NOT EXIST build mkdir build
pushd build
cl %compiler_options% %build_file% %external% /link %linker_options% -subsystem:WINDOWS /out:drift.exe
popd

