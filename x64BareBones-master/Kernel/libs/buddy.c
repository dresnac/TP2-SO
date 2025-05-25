#include "../include/libs/memoryManager.h"
#include <stdint.h>
#include <string.h>

#define LEVELS 13  // log2(HEAP_SIZE / BLOCK_SIZE) + 1

typedef struct Block {
    struct Block *next;
} Block;

typedef struct MemoryManagerCDT {
    uint8_t *base;
    Block *freeLists[LEVELS]; // Array de listas libres por nivel
} MemoryManagerCDT;

static int getLevel(size_t size) {
    size_t s = BLOCK_SIZE;
    int level = 0;
    while (s < size && level < LEVELS - 1) {
        s <<= 1;
        level++;
    }
    return level;
}

static size_t levelSize(int level) {
    return BLOCK_SIZE << level;
}

MemoryManagerADT createMemoryManager(void *const restrict metadata, void *const restrict heapBase) {
    MemoryManagerADT mm = (MemoryManagerADT)metadata;
    mm->base = (uint8_t *)heapBase;

    for (int i = 0; i < LEVELS; i++)
        mm->freeLists[i] = NULL;

    //inicia en el nivel mas alto
    mm->freeLists[LEVELS - 1] = (Block *)mm->base;
    mm->freeLists[LEVELS - 1]->next = NULL;

    return mm;
}

void *allocMemory(MemoryManagerADT const restrict mm, const size_t size) {
    int level = getLevel(size);

    int i = level;
    while (i < LEVELS && mm->freeLists[i] == NULL)
        i++;

    if (i == LEVELS)
        return NULL;

    while (i > level) {
        Block *block = mm->freeLists[i];
        mm->freeLists[i] = block->next;

        size_t halfSize = levelSize(i - 1);
        Block *buddy = (Block *)((uint8_t *)block + halfSize);
        buddy->next = NULL;

        mm->freeLists[i - 1] = block;
        block->next = buddy;
        i--;
    }

    Block *result = mm->freeLists[level];
    mm->freeLists[level] = result->next;
    return (void *)result;
}

static int getBuddyIndex(uint8_t *base, void *addr, size_t size) {
    return ((uintptr_t)addr - (uintptr_t)base) / size;
}

void freeMemory(MemoryManagerADT const restrict mm, void *const restrict ptr) {
    uint8_t *addr = (uint8_t *)ptr;
    int level = 0;
    size_t size = BLOCK_SIZE;

    while (level < LEVELS) {
        size_t offset = (uintptr_t)addr - (uintptr_t)mm->base;
        if (offset % (size << 1) != 0)
            break;
        level++;
        size <<= 1;
    }

    while (level < LEVELS - 1) {
        size_t buddyOffset = ((uintptr_t)addr - (uintptr_t)mm->base) ^ size;
        uint8_t *buddy = mm->base + buddyOffset;

        Block **prev = &mm->freeLists[level];
        Block *curr = mm->freeLists[level];
        while (curr != NULL) {
            if ((uint8_t *)curr == buddy) {
                *prev = curr->next;
                if (addr > buddy) addr = buddy;
                level++;
                size <<= 1;
                goto continue_merge;
            }
            prev = &curr->next;
            curr = curr->next;
        }
        break;
continue_merge:;
    }

    Block *newBlock = (Block *)addr;
    newBlock->next = mm->freeLists[level];
    mm->freeLists[level] = newBlock;
}