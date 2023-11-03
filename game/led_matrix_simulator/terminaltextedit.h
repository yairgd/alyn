/*
 * =====================================================================================
 *
 *       Filename:  terminaltextedit.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/02/2023 09:35:00 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#ifndef TERMINALTEXTEDIT_H
#define TERMINALTEXTEDIT_H
#include <QWidget>
#include <QTextEdit>
#include <QTextBlock>
#include <QDebug>
#include <stdexcept>
#include <QThread>
#include <mutex> 
#include <QTimer>
extern "C" {
	int lua_main (int argc, char **argv);
	void add_history(char *);
	char *  readline(char * promopt);
}

class TerminalTextEdit : public QTextEdit
{
	Q_OBJECT
	public:
		TerminalTextEdit(QWidget *parent = nullptr);
		void insert(const char * text, int l) ;

	protected:
		void keyPressEvent(QKeyEvent *event) override	;	
		void paintEvent(QPaintEvent *event) override ;


	private:
		QTextBlock currentLine;
		QString currentLineText;
		QString prompt;
		int line = 0;
		std::thread lua_thread;
		std::mutex mtx; 
		QTimer * timer;
		
	private slots:	
		void onTimerTimeout() ;	
};

#endif

