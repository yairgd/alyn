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
#ifndef LEDCIRCLE_H
#define LEDCIRCLE_H

#include <QWidget>
#include <iostream>
#include "ledcircle.h"
#include <QtWidgets/QWidget>
#include <QtWidgets/QCheckBox>

class LedCircle : public QWidget
{
    Q_OBJECT

public:
    LedCircle(QWidget *parent = nullptr, int id = 0);
    ~LedCircle();
private:
    int id;
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;

private slots:
  //  void updateLedMatrix( void );
};

#endif // CHESSBOARDWIDGET_H

