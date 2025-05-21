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


using namespace std::chrono_literals;

ThreadedWorker::ThreadedWorker(QObject *parent) : QObject(parent)
{
	moveToThread(&workerThread_);
	workerThread_.start();
	

}


void ThreadedWorker::doWork()
{
	while (true) {
		    emit updateLedMatrix();
		    timer_manage();
		    for (int i = 0; i < 8; i++) 
			    manage_blink(i);
		    QThread::usleep(25000);
		
	}
	

}



