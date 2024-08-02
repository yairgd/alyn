/*
 * =====================================================================================
 *
 *       Filename:  chessboardwidget.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/06/2023 04:58:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

// chessboardwidget.h
#ifndef ARRAWKEY_H
#define ARRAWKEY_H

#include <QWidget>
#include <iostream>
//#include "ledcircle.h"
//#include <QtWidgets/QWidget>

#ifdef _MSC_VER
#include <windows.h>
inline static void usleep(unsigned int x) {
    Sleep(x / 1000);
}
#elif __GNUC__
#include <unistd.h>
 //#include "signals.h"
#endif

#include <chrono>


class ArrawKey : public QWidget
{
    Q_OBJECT

public:
    ArrawKey( int id , QWidget *parent = nullptr);
    ~ArrawKey();
private:
    int id;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;;
    QPixmap img;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private slots:
  //  void updateLedMatrix( void );
};

#endif // CHESSBOARDWIDGET_H

