#define GLProc(name, type) PFNGL##type##PROC gl##name;
#include "opengl_procedure_list.h"

internal void LoadAllOpenGLProcedures(platform *platform)
{
#define GLProc(name, type) gl##name = (PFNGL##type##PROC)platform->LoadOpenGLProcedure("gl" #name);
#include "opengl_procedure_list.h"
}
