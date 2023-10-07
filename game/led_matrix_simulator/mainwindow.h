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
#include "ui_mainwindow.h"
class ChessboardWidget; // Forward declaration

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui; // Declare the ui objecy
    ChessboardWidget *chessboardWidget; // Pointer to the ChessboardWidget

    // Other private members and methods as needed
};

#endif // MAINWINDOW_H

