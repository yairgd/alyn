#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ledmatrix.h"
#include "ledcircle.h"
#include "system_model.h"
#include "arrawkey.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	  , ui(new Ui::MainWindow)
{
	ui->setupUi(this);



	led1 = new LedCircle(ui->led1, 1);
	led2 = new LedCircle(ui->led2, 2);
	led3 = new LedCircle(ui->led3, 3);
	led4 = new LedCircle(ui->led4, 4);
	led5 = new LedCircle(ui->led5, 5);
	led6 = new LedCircle(ui->led6, 6);
	led7 = new LedCircle(ui->led7, 7);
	led8 = new LedCircle(ui->led8, 8);



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



	QVBoxLayout *layout = new QVBoxLayout(ui->keypad);
	layout->addWidget( new ArrawKey(0));
	layout->addWidget( new ArrawKey(1));
	layout->addWidget( new ArrawKey(2));
	layout->addWidget( new ArrawKey(3));
	layout->addWidget( new ArrawKey(4));

	/*

	auto * l1 = new ArrawKey(ui->k1, 0);
	ui->k1->setGeometry(200,50,ui->k1->parentWidget()->width()/3, ui->k1->parentWidget()->height()/3);
	auto * l2 = new ArrawKey(ui->k2, 1);
	ui->k2->setGeometry(150,50,ui->keypad->width()/3, ui->keypad->height()/3);
*/
}

MainWindow::~MainWindow()
{
	delete ui;
}

