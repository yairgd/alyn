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
#include "ledcircle.h"
#include <QtWidgets/QWidget>

class ArrawKey : public QWidget
{
    Q_OBJECT

public:
    ArrawKey( int id , QWidget *parent = nullptr);
    ~ArrawKey();
private:
    int id;
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private slots:
  //  void updateLedMatrix( void );
};

#endif // CHESSBOARDWIDGET_H

