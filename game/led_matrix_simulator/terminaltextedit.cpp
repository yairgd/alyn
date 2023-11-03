/*
 * =====================================================================================
 *
 *       Filename:  TerminalTextEdit.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/02/2023 09:39:25 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "terminaltextedit.h"
#include <csignal>
#include <iostream>


namespace
{
	volatile std::sig_atomic_t gSignalStatus;
}

void signal_handler(int signal)
{
	gSignalStatus = signal;
}


static TerminalTextEdit * textedit;


#ifdef USE_GUI_CONSOLE
static int line_ok =0;
static char * line_str ;
extern "C" {
	// this code below is used when GUI functions replace the readline library
	void add_history(char * line)
	{
	}


	/**
	 * Created  11/04/2023
	 * @brief   this function sis cllabck from lua.c:436  
	 * @note  
	 * @param   
	 * @return  
	 */
	char *  readline(char * promopt)
	{
		while (!line_ok);
		line_ok = 0;
		//qDebug()<<line_str;
		return 	line_str;
	}
	char * rl_readline_name;

	// this functions are iplemeted below and used here: lauxlib.h:261 (not part of the lua src code - yair add it)	
	void write_line(const char * s, int l) {
		textedit->insert(s,l);
	}

	void write_format_line(char * s,const char * p) 
	{
		char str[1024];
		snprintf(str, sizeof(str), s, p);
		write_line(str,0) ;
		write_line("\n",0) ;

	}
}
#endif

TerminalTextEdit::TerminalTextEdit(QWidget *parent) : QTextEdit(parent)
{
	// setReadOnly(true);  // Make the text edit read-only
	setLineWrapMode(QTextEdit::NoWrap);  // Disable line wrapping

	// Install an event filter to handle mouse events
	//installEventFilter(this);

	// Initialize the current command line
	currentLine = document()->findBlockByNumber(line);
	currentLineText = currentLine.text();
	prompt = "lua $ ";

	// Display the initial prompt
	insertPlainText(prompt);

	// Set the cursor to the end of the document
	QTextCursor cursor = textCursor();
	cursor.movePosition(QTextCursor::End);
	setTextCursor(cursor);

	const char* argv[] = {const_cast<char*> ("program_name"),
		//     const_cast<char*> ("-arg1"),
		//     const_cast<char*> ("string_to_arg1"),
		//     const_cast<char*> ("-arg2"),
		//     const_cast<char*>("-arg3"),
		NULL};
	int argc = sizeof (argv) / sizeof (char*) - 1;

	lua_thread = std::thread ([argc, argv]()  {
		lua_main(argc,const_cast<char**>(argv));	
	});

	

	timer = new QTimer(this);
	        connect(timer, &QTimer::timeout, this, &TerminalTextEdit::onTimerTimeout);
	timer->start(100);

	std::signal(SIGINT, signal_handler);
	textedit = this;
}

void TerminalTextEdit::onTimerTimeout() {
			// This method is called when the timer times out
			// You can perform actions here at regular intervals
			 timer->start(100); // Start the timerL
			repaint();
			update();

		}
void TerminalTextEdit::insert(const char * text, int l) 
{
	//const std::lock_guard<std::mutex> lock(mtx);

	QTextCursor cursor = textCursor();

	// Handle the Enter key press

	cursor.movePosition(QTextCursor::End);
	setTextCursor(cursor);

	if (text[0] != '\n') {
		insertPlainText(text); // Display the prompt					 
// 
	} else {

		insertPlainText("\n"); // Add a new line			
		insertPlainText(prompt); // Display the prompt
	}
	cursor.movePosition(QTextCursor::End);
	setTextCursor(cursor);			

	line =  document()->lineCount()  - 1;

	currentLine = document()->findBlockByNumber(line);
	

}
void TerminalTextEdit::keyPressEvent(QKeyEvent *event) 
{


	if (event->key() == Qt::Key_Return)
	{
#ifdef USE_GUI_CONSOLE
		QString cmd;
		do {
			QTextBlock currentLine = document()->findBlockByNumber(line);

			line++;
			// QString targetBlockText = currentLine.text();
			cmd += currentLine.text();
			cmd += "\n";
			// qDebug()<<currentLine.text();

		} while(line < document()->lineCount() );




		// Get the text of the specific block (line)
		const std::string& stdS = cmd.toStdString();		
		const char *cString = stdS.c_str(); // cmd.toUtf8().constData();
		line_str = (char*)malloc(stdS.length());
		if (line_str)
			memcpy (line_str,cString + prompt.length(), stdS.length());

		//const std::lock_guard<std::mutex> lock(mtx);		
		QTextCursor cursor = textCursor();
		cursor.movePosition(QTextCursor::EndOfLine);			
		setTextCursor(cursor);
		
		
		// Handle the Enter key press
		insertPlainText("\n"); // Add a new line

		insertPlainText(prompt); // Display the prompt
					 //
					 //
		cursor.movePosition(QTextCursor::End);
		setTextCursor(cursor);
		line =  document()->lineCount()  - 1;

		currentLine = document()->findBlockByNumber(line);



		line_ok  = 1;
#endif
		

	} else if (event->key() == Qt::Key_Up) {
		// ignore key up , manage history here	
		qDebug() << "key up";

	} else if (event->key() == Qt::Key_Down) {
		qDebug() << "key down";

	} else if (event->key() == Qt::Key_Home || (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_A) ) {
		QTextCursor cursor = textCursor();
		cursor.movePosition(QTextCursor::StartOfLine);
		for (int i = 0; i < prompt.length(); i++) 
			cursor.movePosition(QTextCursor::Right);	

		setTextCursor(cursor);				

	} else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_L) {
		clear();
		line = 0;
		currentLine = document()->findBlockByNumber(0);	
		insertPlainText(prompt);

	}
	else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_C) {
		copy();
	} else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_D) {
		std::raise(SIGINT);
		std::signal(SIGINT, signal_handler);
	}
	else if (event->key() == Qt::Key_Backspace) {
		QTextCursor cursor = textCursor();
		// ignore backspase if it delte the prompt
		if (cursor.positionInBlock() >  prompt.length()) {
			QTextEdit::keyPressEvent(event);						
		}						

	}
	else 
	{
		if (event->modifiers() != Qt::ControlModifier || event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_V) {
			QTextCursor cursor = textCursor();				
			if (cursor.block() != currentLine)
			{ 		
				cursor.movePosition(QTextCursor::End);
				setTextCursor(cursor);					
			}
		}


		// move to end of line
		QTextCursor cursor = textCursor();
		cursor.movePosition(QTextCursor::EndOfLine);				
		setTextCursor(cursor);				

		// Handle other key presses
		QTextEdit::keyPressEvent(event);
	}
}



void TerminalTextEdit::paintEvent(QPaintEvent *event)  
{
	QTextEdit::paintEvent(event);
	
	setGeometry(0,0,parentWidget()->width(), parentWidget()->height());

}


