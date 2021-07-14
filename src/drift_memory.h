#ifndef MEMORY_H 
#define MEMORY_H

typedef struct memory_arena
{
    void *memory;
    u32 memory_size;
    u32 position;
} memory_arena;

internal memory_arena InitMemArena(void *memory, u32 size);
internal memory_arena InitMemSubArena(memory_arena *container, u32 size);
internal void ClearMemArena(memory_arena *arena);
internal void *AllocateMemArena(memory_arena *arena, u32 size);

#endif
