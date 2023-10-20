/*
 * =====================================================================================
 *
 *       Filename:  ThreadedWorker.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/12/2023 04:25:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "threadedworker.h"
#include <chrono>
#include "system_model.h"

#ifdef __cplusplus
extern "C" {
#endif
	int lua_main (int argc, char **argv);

#ifdef __cplusplus
}
#endif
using namespace std::chrono_literals;

ThreadedWorker::ThreadedWorker(QObject *parent) : QObject(parent)
{
	moveToThread(&workerThread_);
	workerThread_.start();
	

}


void ThreadedWorker::doWork()
{

    // Emit a signal with the result
//    emit resultReady("Woker starts lua engine");




	
	const char* argv[] = {const_cast<char*> ("program_name"),
		//     const_cast<char*> ("-arg1"),
		//     const_cast<char*> ("string_to_arg1"),
		//     const_cast<char*> ("-arg2"),
		//     const_cast<char*>("-arg3"),
		NULL};
	int argc = sizeof (argv) / sizeof (char*) - 1;

	std::thread t1 ([argc, argv]()  {
		lua_main(argc,const_cast<char**>(argv));	
	});

	

	while (true) {
		    emit updateLedMatrix();
		    for (int i = 1; i <= 8; i++) 
			    manage_blink(i);
		    QThread::usleep(25000);
		
	}
	

}



