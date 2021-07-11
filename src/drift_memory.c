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

internal void ClearMemArena(memory_arena *arena)
{
    arena->position = 0;
}

internal void *AllocateMemArena(memory_arena *arena, u32 size)
{
    void *memory = 0;

    if (arena->position + size <= arena->memory_size)
    {
        memory = (char *)arena->memory + arena->position;
        arena->position += size;
    }

    return memory;
}

internal memory_sub_arena InitMemSubArena(memory_arena *arena, u32 size)
{
    memory_sub_arena sub_arena = {0};
    {
        sub_arena.container = arena;
        sub_arena.memory_size = size;
        sub_arena.offset = arena->position;
        sub_arena.position = arena->position;
    }

    return sub_arena;
}    

internal void ClearMemSubArena(memory_sub_arena *sub_arena)
{
    sub_arena.position = 0;
}

