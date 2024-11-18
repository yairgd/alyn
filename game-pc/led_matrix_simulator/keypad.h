/*
 * =====================================================================================
 *
 *       Filename:  keypad.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/14/23 02:15:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef KEYPAD_H
#define KEYPAD_H 
#include <QWidget>
#include <iostream>
#include "arrawkey.h"
#include <QtWidgets/QWidget>
#include <QtWidgets>

class Keypad : public QWidget
{
    Q_OBJECT

public:
    Keypad( QWidget *parent = nullptr);
    ~Keypad();
private:
	ArrawKey * keys[5] = {0};
	QVBoxLayout *layout;
protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
  //  void updateLedMatrix( void );
};
#endif
