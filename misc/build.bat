@echo off

set stb=..\..\..\stb
set src=..\drift_glad_test
set gl=..\..\..\glext
set gl_lib=..\..\..\glext\lib\glext.lib
set glad=..\..\..\glad

set compiler_options= -DDRIFT_SLOW=1 /nologo /FC /Zi /MD
set linker_options= user32.lib gdi32.lib Shell32.lib winmm.lib opengl32.lib 
set external= -I%src% -I%stb% -I%gl% -I%glad%\include

set build_file=../drift_glad_test/win32_drift.c %glad%\src\glad.c

IF NOT EXIST build mkdir build
pushd build
cl %compiler_options% %build_file% %external% /link %linker_options% -subsystem:WINDOWS /out:drift.exe
popd

