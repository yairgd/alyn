/*
 * =====================================================================================
 *
 *       Filename:  lua_memory.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/07/2024 07:31:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <zephyr/sys/mem_stats.h>

#include "utils/lua_memory.h"


/* 
 * Implemented a custom heap for Lua's source code, 
 * enabling the utilization of malloc and free for its API 
 **/
//K_HEAP_DEFINE(heap, 64536);

static uint8_t lua_heap_mem[64536] __aligned(sizeof(void *));
static struct sys_heap heap;

/* MUST be called once before Lua starts */
void lua_mem_init(void)
{
    sys_heap_init(&heap, lua_heap_mem, sizeof(lua_heap_mem));
}

void *lua_malloc(size_t n)
{
    if (n == 0) {
        return NULL;
    }

    void *ptr = sys_heap_alloc(&heap, n);
    if (!ptr) {
        printk("LUA OOM: alloc %zu failed\n", n);
    }
    return ptr;
}

char *lua_realloc(char *ptr, size_t n)
{
    /* realloc(NULL, n) == malloc(n) */
    if (ptr == NULL) {
        return lua_malloc(n);
    }

    /* realloc(ptr, 0) == free(ptr) */
    if (n == 0) {
        sys_heap_free(&heap, ptr);
        return NULL;
    }

    char *new_ptr = sys_heap_realloc(&heap, ptr, n);
    if (!new_ptr) {
        printk("LUA OOM: realloc %zu failed\n", n);
    }
    return new_ptr;
}

void lua_free(void *ptr)
{
    if (ptr) {
        sys_heap_free(&heap, ptr);
    }
}


/* this section is not usesfull, I lived it here as an example to memory slab */
#include "common/effects/src/frame.h"
#include <stdio.h>
#define MAX_FRAMES 6
K_MEM_SLAB_DEFINE(frame_slab_mem, sizeof(struct frame), MAX_FRAMES, 4);
struct frame * lua_mem_frame_malloc() {
	struct frame * block_ptr = 0;

	if (k_mem_slab_alloc(&frame_slab_mem, (void **)&block_ptr, K_MSEC(100)) == 0) {
		memset(block_ptr, 0, sizeof(struct frame));
	} else {
		printf("Frame Memory allocation time-out");
	}
	return block_ptr;
}


#include "common/effects/src/banner.h"
#define MAX_BANNERS 6
K_MEM_SLAB_DEFINE(banner_slab_mem, sizeof(struct banner), MAX_BANNERS, 4);
struct banner * lua_mem_banner_malloc() {
	struct banner * block_ptr = 0;

	if (k_mem_slab_alloc(&banner_slab_mem, (void **)&block_ptr, K_MSEC(100)) == 0) {
		memset(block_ptr, 0, sizeof(struct banner));
	} else {
		printf("Frame Memory allocation time-out");
	}
	return block_ptr;
}

void lua_mem_banner_free(struct banner * block_ptr) {
	k_mem_slab_free(&banner_slab_mem, (void *)&block_ptr);
}


void lua_mem_frame_free(struct frame * block_ptr) {
	k_mem_slab_free(&frame_slab_mem, (void *)&block_ptr);
}

