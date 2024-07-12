/*
 * =====================================================================================
 *
 *       Filename:  mainwindow.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/06/2023 05:36:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <thread>
#include "ui_mainwindow.h"
#include "ledcircle.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "luasrc.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#ifdef __cplusplus
}
#endif

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow(QWidget *parent = nullptr);
		~MainWindow();
		void onStartStopClicked();

	private:
		Ui::MainWindow *ui; // Declare the ui objecy
				    //   ChessboardWidget *chessboardWidget; // Pointer to the ChessboardWidget

		LedCircle * led1 ;
		LedCircle * led2 ;
		LedCircle * led3 ;
		LedCircle * led4 ;
		LedCircle * led5 ;
		LedCircle * led6 ;
		LedCircle * led7 ;
		LedCircle * led8 ;
		bool isGameRun = false;
		lua_State *L = 0;
		std::thread lua_thread;

		
protected:
};

#endif // MAINWINDOW_H

