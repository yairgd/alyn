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
#ifndef CHESSBOARDWIDGET_H
#define CHESSBOARDWIDGET_H

#include <QWidget>
#include "led_matrix.h"

class LedMatrixWidget : public QWidget
{
    Q_OBJECT

public:
    LedMatrixWidget(QWidget *parent = nullptr);
    ~LedMatrixWidget();
private:
    struct led_matrix led_matrix = {0};
protected:
    void paintEvent(QPaintEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
};

#endif // CHESSBOARDWIDGET_H

