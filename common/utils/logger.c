/*
 * =====================================================================================
 *
 *       Filename:  logger.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/03/2023  01:50:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Israel
 *
 * =====================================================================================
 */
#include "logger.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#ifdef _MSC_VER
#elif __GNUC__
#include <unistd.h>
#endif


int log_facility;
static char *domain_name;
static CODE prioritynames[] =
  {
   // { (char*)"err", LOG_ERR },
    { (char*)"warning", LOG_WARNING },
    { (char*)"notice", LOG_NOTICE },
    { (char*)"info", LOG_INFO },
    { (char*)"debug", LOG_DEBUG },
    { NULL, -1 }
  };
static int log_level = LOG_DEBUG;


const char* logger_get_process_name()
{
    char* name = 0;
    if (!name)
	    name = (char*)calloc(1024,sizeof(char));
    if (domain_name)
	    return domain_name;

    if(name){
        sprintf(name, "/proc/%d/cmdline",getpid());
        FILE* f = fopen(name,"r");
        if(f){
            size_t size;
            size = fread(name, sizeof(char), 1024, f);
            if(size>0){
                if('\n'==name[size-1])
                    name[size-1]='\0';
            }
            fclose(f);
        }
    }
    return name;
}



/**
 * Created  02/03/2023
 * @brief   checks if the log is allowed to be displayed by checking the current allowed log level to the required one
 * LOG_ERR          3      
 * LOG_WARNING      4        
 * LOG_NOTICE       5        // trace
 * LOG_INFO         6      
 * LOG_DEBUG        7        
 * @param   
 * @return  1 - allowd , 0 not alloed
 */
int logger_logstatus(int level)
{
	if (level > log_level) {
		return 0;
	}
	return 1;
}

const char *logger_logname(int c_val)
{
	CODE * priorityname =  prioritynames;
	while (priorityname->c_name && priorityname->c_val != c_val) {
		priorityname++;
	};
	return priorityname->c_name;
}



void logger_init(char *_domain_name,int _log_facility)
{
	log_facility= _log_facility;
	/* use to overide domain name - @see logger_get_process_name */ 

	domain_name = 0;
	if (_domain_name)
		domain_name = strdup (_domain_name);
 }

void logger_set_log_level(int level) {
	log_level = level;
}
void logger_free()
{
/* empty */
}


