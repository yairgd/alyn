#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ledmatrix.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create and set LedMatrixWidget as the central widget
    LedMatrixWidget *ledMatrixWidget = new LedMatrixWidget(ui->display);
   ledMatrixWidget->setGeometry(0,0,640,320);

}

MainWindow::~MainWindow()
{
    delete ui;
}

