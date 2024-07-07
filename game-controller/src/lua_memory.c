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

#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <zephyr/sys/mem_stats.h>

#include "utils/lua_memory.h"


/* 
 * Implemented a custom heap for Lua's source code, 
 * enabling the utilization of malloc and free for its API 
 **/
static char lua_heap_mem[32768];
static struct  sys_heap heap;
char * lua_realloc(char *ptr, size_t n ) {
	  return sys_heap_realloc(&heap,ptr,n);

}
void lua_free(void *ptr) {
	sys_heap_free(&heap,ptr);

}


void * lua_malloc(size_t n ) {
	return sys_heap_alloc(&heap,  n);

}

void lua_mem_init() {
	sys_heap_init (&heap, lua_heap_mem, sizeof(lua_heap_mem));	
}
