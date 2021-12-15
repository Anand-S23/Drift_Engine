#include "drift_memory.h"

internal buffer InitBuffer(void *memory, u32 size)
{
    return (buffer) {
        .data = memory,
        .size = size,
        .offset = 0,
        .position = 0
    };
}
    
#define BufferRead(__buffer, __type, __val_p)                   \
do {                                                            \
    buffer_t *__b = __buffer;                                   \
    __type *val_p = (__type *)(__val_p);                        \
    *(val_p) = *(__type *)(__b->data + __b->position);          \
    __b->position += sizeof(__type);                            \
} while (0)

#define BufferWrite(__buffer, __type, __val)                    \
do {                                                            \
    buffer_t *__b = (__buffer);                                 \
    size_t data_size = sizeof(__type);                          \
    size_t potential_size = __b->position + data_size;          \
    *(__type *)(__b->data + __b->position) = __val;             \
    __b->used += (u32)data_size;                                \
    __b->position += (u32)data_size;                            \
} while (0)

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
