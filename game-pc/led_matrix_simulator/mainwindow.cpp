#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ledmatrix.h"
#include "ledcircle.h"
#include "system_model.h"
#include "keypad.h"

#include <QtWidgets>
#include <QComboBox>
#include <thread>
#include "terminaltextedit.h"

#include "luasrc.h"
#include "utils/ThreadPool.h"


#ifdef _MSC_VER
#  include <windows.h>
#else
#  include <pthread.h>
#endif

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	  , ui(new Ui::MainWindow)
{
	ui->setupUi(this);



	led1 = new LedCircle(ui->led1, 0);
	led2 = new LedCircle(ui->led2, 1);
	led3 = new LedCircle(ui->led3, 2);
	led4 = new LedCircle(ui->led4, 3);
	led5 = new LedCircle(ui->led5, 4);
	led6 = new LedCircle(ui->led6, 5);
	led7 = new LedCircle(ui->led7, 6);
	led8 = new LedCircle(ui->led8, 7);



	QObject::connect(ui->P1, &QPushButton::clicked, ui->P1, []() {set_button_state(0,1);});
	QObject::connect(ui->P2, &QPushButton::clicked, ui->P2, []() {set_button_state(1,1);});
	QObject::connect(ui->P3, &QPushButton::clicked, ui->P3, []() {set_button_state(2,1);});
	QObject::connect(ui->P4, &QPushButton::clicked, ui->P4, []() {set_button_state(3,1);});
	QObject::connect(ui->P5, &QPushButton::clicked, ui->P5, []() {set_button_state(4,1);});
	QObject::connect(ui->P6, &QPushButton::clicked, ui->P6, []() {set_button_state(5,1);});
	QObject::connect(ui->P7, &QPushButton::clicked, ui->P7, []() {set_button_state(6,1);});
	QObject::connect(ui->P8, &QPushButton::clicked, ui->P8, []() {set_button_state(7,1);});


	/* this add compiled lua src's to linked list */
	for (int i = 0; i < luasrc_size() && i < 16; i++) {	
		const struct luasrc * src = luasrc_by_idx(i);
		ui->gameList->addItem(src->name);
	}
	connect(ui->startStop, &QPushButton::clicked, this, &MainWindow::onStartStopClicked);




	// Create and set LedMatrixWidget as the central widget
	LedMatrixWidget *ledMatrixWidget = new LedMatrixWidget(ui->display, std::array<LedCircle *,8>({led1,led2,led3,led4,led5,led6,led7,led8}));
#if 1
	auto keypad = new QWidget();

	//ui->keypad->addWidget(new QPushButton("test"));
	ui->keypad->addWidget(keypad);


	new Keypad(keypad);
#else
	//ui->keypad->addWidget(keypad)

	//auto k = new Keypad( );
	//	ui->keypad->addWidget(k);
	//	k->setParent(ui->keypad);

	ui->keypad->addWidget(new Keypad( ));

	//ui->keypad->addWidget(new Keypad(ui->keypad));
	//new Keypad(ui->keypad->parentWidget());
#endif

	new TerminalTextEdit(ui->tab);
	/*

	   auto * l1 = new ArrawKey(ui->k1, 0);
	   ui->k1->setGeometry(200,50,ui->k1->parentWidget()->width()/3, ui->k1->parentWidget()->height()/3);
	   auto * l2 = new ArrawKey(ui->k2, 1);
	   ui->k2->setGeometry(150,50,ui->keypad->width()/3, ui->keypad->height()/3);
	   */
}

void MainWindow::onStartStopClicked()
{
	if (! isGameRun) {
		lua_thread = std::thread([this]()->void{
			int index = ui->gameList->currentIndex();
			const struct luasrc * game =  luasrc_by_idx(index);;
			if (L)
				lua_close(L);
			L = luaL_newstate();
			luaL_openlibs(L);
			int error =
				luaL_loadbuffer( L, game->code, game->size, "main" )
				|| lua_pcall( L, 0, 0, 0 );

			if (error) {
				fprintf(stderr, "%s\n", lua_tostring(L,1));

			}
		});
		isGameRun = true;
		ui->startStop->setText("Stop");
		ui->gameList->setEnabled(false);		
	} else {
		isGameRun = false;
		ui->startStop->setText("start");
		ui->gameList->setEnabled(true);
		auto handle = lua_thread.native_handle();
#ifdef _MSC_VER
		TerminateThread(handle, 0);
#else
		pthread_cancel(handle);
#endif
		lua_thread.detach();

	}

}

MainWindow::~MainWindow()
{
	delete ui;
}





