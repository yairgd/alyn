#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chessboardwidget.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create and set ChessboardWidget as the central widget
    ChessboardWidget *chessboardWidget = new ChessboardWidget(ui->display);
   chessboardWidget->setGeometry(0,0,640,320);

 //  setCentralWidget(chessboardWidget);
//    ui->display->setParent (chessboardWidget);

    //QGridLayout *layout = new QGridLayout;
//	layout->addWidget(chessboardWidget, 0, 1);
  //  setLayout(layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

