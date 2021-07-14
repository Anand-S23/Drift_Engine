#include "drift_memory.h"

internal memory_arena InitMemArena(void *memory, u32 size)
{
    memory_arena arena = {0};
    {
        arena.memory = memory;
        arena.memory_size = size;
        arena.position = 0;
    }

    return arena;
}

internal memory_arena InitMemSubArena(memory_arena *container, u32 size)
{
    memory_arena arena = {0};
    {
        arena.memory = (u8 *)container->memory + container->position;
        arena.memory_size = size;
        arena.position = 0;
    }

    return arena;
}

internal void ClearMemArena(memory_arena *arena)
{
    arena->position = 0;
}

internal void *AllocateMemArena(memory_arena *arena, u32 size)
{
    void *memory = 0;

    if (arena->position + size <= arena->memory_size)
    {
        memory = (u8 *)arena->memory + arena->position;
        arena->position += size;
    }

    return memory;
}

#define D_Malloc(type, arena, size) (type *)AllocateMemArena(arena, size)
#define D_Free(arena) ClearMemArena(ClearMemArena)
