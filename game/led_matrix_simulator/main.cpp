/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/06/2023 05:27:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include <QApplication>
#include <QThread>
#include <QDebug>  

#include "mainwindow.h"
#include "threadedworker.h"
#include <exception>
#include <qnamespace.h>
#include <thread>
#include <ostream>
#include <iostream>
#include <sstream>

#ifdef __cplusplus
extern "C" {
#endif
	int lua_main (int argc, char **argv);

#ifdef __cplusplus
}
#endif

//https://stackoverflow.com/questions/4810516/c-redirecting-stdout
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

#if 0
	ThreadedWorker worker;
	QThread thread;
	thread.start();

	worker.moveToThread(&thread);

// Connect the worker's signal to a slot in the main thread
    QObject::connect(&worker, &ThreadedWorker::resultReady, [argc, argv](const QString &result) {
        qDebug() << "Main thread: " << result;
        lua_main(argc,argv);
    });


    // Start the thread
    thread.start();

    // Trigger the worker's slot from the main thread
    QMetaObject::invokeMethod(&worker, "doWork", Qt::QueuedConnection);
#endif
    
  // Connect the worker's signal to a slot in the main thread
//    QObject::connect(&worker, &worker::resultReady, [](const QString &result) {
 //      lua_main(argc,argv);
 //   });
	//worker.start();

//	std::thread t1 ([argc, argv]()  {
///		lua_main(argc,argv);
//	});
//	t1.detach();
	return a.exec();





}

