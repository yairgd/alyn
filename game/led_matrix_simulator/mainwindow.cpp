#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ledmatrix.h"
#include "ledcircle.h"
#include "system_model.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	  , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

		//ledMatrixWidget->setGeometry(0,0,640,320);


	led1 = new LedCircle(ui->led1, /*ui->active1,*/ 1);
	led2 = new LedCircle(ui->led2, /*ui->active2,*/ 2);
	led3 = new LedCircle(ui->led3, /*ui->active3,*/ 3);
	led4 = new LedCircle(ui->led4, /*ui->active4,*/ 4);
	led5 = new LedCircle(ui->led5, /*ui->active5 ,*/5);
	led6 = new LedCircle(ui->led6, /*ui->active6,*/ 6);
	led7 = new LedCircle(ui->led7, /*ui->active7, */7);
	led8 = new LedCircle(ui->led8, /*ui->active8,*/ 8);

/*
	QObject::connect(ui->led1, SIGNAL(clicked()), ui->led1, SLOT(update()));
	QObject::connect(ui->active2, SIGNAL(clicked()), ui->led2, SLOT(update()));
	QObject::connect(ui->active3, SIGNAL(clicked()), ui->led3, SLOT(update()));
	QObject::connect(ui->active4, SIGNAL(clicked()), ui->led4, SLOT(update()));
	QObject::connect(ui->active5, SIGNAL(clicked()), ui->led5, SLOT(update()));
	QObject::connect(ui->active6, SIGNAL(clicked()), ui->led6, SLOT(update()));
	QObject::connect(ui->active7, SIGNAL(clicked()), ui->led7, SLOT(update()));
	QObject::connect(ui->active8, SIGNAL(clicked()), ui->led8, SLOT(update()));
*/
	/*auto func =[=](int id )->void {
		set_button_state(1);	
	};*/

	QObject::connect(ui->P1, &QPushButton::clicked, ui->P1, []() {set_button_state(1,1);});
	QObject::connect(ui->P2, &QPushButton::clicked, ui->P2, []() {set_button_state(2,1);});
	QObject::connect(ui->P3, &QPushButton::clicked, ui->P3, []() {set_button_state(3,1);});
	QObject::connect(ui->P4, &QPushButton::clicked, ui->P4, []() {set_button_state(4,1);});
	QObject::connect(ui->P5, &QPushButton::clicked, ui->P5, []() {set_button_state(5,1);});
	QObject::connect(ui->P6, &QPushButton::clicked, ui->P6, []() {set_button_state(6,1);});
	QObject::connect(ui->P7, &QPushButton::clicked, ui->P7, []() {set_button_state(7,1);});
	QObject::connect(ui->P8, &QPushButton::clicked, ui->P8, []() {set_button_state(8,1);});



	// Create and set LedMatrixWidget as the central widget
	LedMatrixWidget *ledMatrixWidget = new LedMatrixWidget(ui->display, std::array<LedCircle *,8>({led1,led2,led3,led4,led5,led6,led7,led8}));


}

MainWindow::~MainWindow()
{
	delete ui;
}

