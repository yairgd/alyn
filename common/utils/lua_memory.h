/*
 * =====================================================================================
 *
 *       Filename:  lua_memory.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/18/24 16:32:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef LUA_MEMORY
#define LUA_MEMORY 
#include "stdlib.h"

char * lua_realloc(char *ptr, size_t n );
void lua_free(void *ptr);
void * lua_malloc(size_t n );
void lua_mem_init();


struct frame * lua_mem_frame_malloc();
void lua_mem_frame_free(struct frame * block_ptr);


struct banner * lua_mem_banner_malloc();
void lua_mem_banner_free(struct banner * block_ptr);


#endif
