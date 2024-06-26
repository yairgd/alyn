/*
 * =====================================================================================
 *
 *       Filename:  memory.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/15/2024 10:14:45 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "lua_memory.h"
/** @defgroup LuaSrc Lua 5.4.6 Sources */
/** @{ */

/**
 * Created  02/15/2024
 * @brief This function serves as a wrapper for realloc within the Lua API. This replacement is essential to facilitate compatibility with the Zephyr HAL. see lauxlib.c:1027
 * @note  
 * @param   
 * @return  
 */
char * lua_realloc(char *ptr, size_t n ) {
	return realloc(ptr, n);
}

/**
 * Created  02/15/2024
 * @brief This function serves as a wrapper for free within the Lua API. This replacement is essential to facilitate compatibility with the Zephyr HAL.  see lauxlib.c:1023
 * @note  
 * @param   
 * @return  
 */
void lua_free(void *ptr) {
	free(ptr);
}



/**
 * Created  06/18/24
 * @brief   not used by LUA api, used by the application 
 * @param   
 * @return  
 */
void * lua_malloc(size_t n ) {
	return malloc(n);
}
/** @} */
