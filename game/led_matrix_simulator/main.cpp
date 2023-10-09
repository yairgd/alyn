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
#include "mainwindow.h"
#include <exception>
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




	std::thread t1 ([argc, argv]()  {
		lua_main(argc,argv);

	});
	t1.detach();
	a.exec();



		return 0;


}

