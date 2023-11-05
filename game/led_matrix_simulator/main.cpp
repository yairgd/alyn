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
//#include <QThread>
#include <QDebug>  

#include <exception>
#include <qnamespace.h>
#include <thread>
#include <ostream>
#include <iostream>
#include <sstream>

#include "mainwindow.h"
#include "threadedworker.h"


#include "led_matrix.h"


#ifdef __cplusplus
extern "C" {
#endif
	int lua_main (int argc, char **argv);

#ifdef __cplusplus
}
#endif


#include "luasrc.h"

//https://stackoverflow.com/questions/4810516/c-redirecting-stdout
int main(int argc, char *argv[])
{

		struct luasrc * l = luasrc_by_name("_home_yair_alyn_project_Debug_game_lua_5_4_6_demo1_lua"); 
	struct luasrc * l1 = luasrc_by_name("ddd"); 
	
	QApplication a(argc, argv);
	MainWindow w;
	led_matrix_init(led_matrix_get(), 64,32);
	
	w.show();
 
return a.exec();
}

