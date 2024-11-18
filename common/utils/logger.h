/*
 * =====================================================================================
 *
 *       Filename:  logger.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/03/2023 01:46:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
void logger_free();
void logger_init(char *domain_name,int log_type);
const char* logger_get_process_name();
void logger_set_log_level(int level) ;
int logger_logstatus(int level);
const char *logger_logname(int level);
#ifdef __cplusplus
}
#endif
extern int log_facility;



// taken from syslog.h
/*
  LOG_ERR          3      
  LOG_WARNING      4        
  LOG_NOTICE       5        // trace
  LOG_INFO         6      
  LOG_DEBUG        7        
*/
#ifdef NO_SYSLOG
//#  define LOG_ERR     3
# define LOG_WARNING 4
# define LOG_NOTICE  5
# define LOG_INFO    6
# define LOG_DEBUG   7
#else
# include <syslog.h>
#endif

typedef struct _code {
        char        *c_name;
        int        c_val;
} CODE;



#ifdef LINUX_PC
#	include <sys/types.h>
#	include <unistd.h>
#   include <syslog.h>
#	define logger_tosysylog(log_type, fmt, ...) \
		openlog(logger_get_process_name() ,LOG_PID|LOG_NDELAY ,log_facility);\
		syslog (log_type, "%s:%s:%d:%s(): " fmt, logger_logname(log_type),__FILE__, __LINE__, __func__, ##__VA_ARGS__); \
		closelog();
#else
#	define	logger_tosysylog(log_type, fmt, ...)
#endif

#define logger_print(log_type,fmt, ...)\
        do {   \
		if (logger_logstatus(log_type)) { \
			logger_tosysylog(log_type, fmt, ##__VA_ARGS__); \
			printf ("[%s:%s:%d:%s()]: " fmt "\n", logger_logname(log_type), __FILE__, __LINE__, __func__, ##__VA_ARGS__);\
		}\
	}\
	while (0);



#endif
