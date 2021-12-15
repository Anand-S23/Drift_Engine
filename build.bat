@echo off

set stb=..\external\stb
set g=..\external\glad
set gl=..\external\glext
set src=..\src

set compile_flags= -DDRIFT_SLOW=1 -nologo -FC -MT -Z7 -GR-
set common_link_flags= opengl32.lib -opt:ref -incremental:no /Debug:fastlink
set platform_link_flags= gdi32.lib user32.lib winmm.lib shell32.lib kernel32.lib %common_link_flags%
set external= %g%\src\glad.c -I%src% -I%stb% -I%g%\include -I%gl%

set src_file=../src/app.c
set engine_file=../src/platform/win32_drift.c

IF NOT EXIST build mkdir build
pushd build
del *.pdb > NUL 2> NUL
cl %compile_flags% %engine_file% %external% /link %platform_link_flags% -out:app.exe
cl %compile_flags% %src_file% %external% -LD /link %common_link_flags%  -PDB:drift_%random%.pdb -out:drift.dll 
popd

