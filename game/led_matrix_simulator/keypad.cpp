/*
 * =====================================================================================
 *
 *       Filename:  keypad.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/14/23 02:15:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#include "keypad.h"
#include <QPainter>
#include <QDebug>
#include <QtWidgets>

Keypad::Keypad(QWidget *parent)
	: QWidget(parent)
{

	layout = new QVBoxLayout(this);
	
	layout->addWidget( (keys[0] = new ArrawKey(0)));
	layout->addWidget( (keys[1] = new ArrawKey(1)));
	layout->addWidget( (keys[2] = new ArrawKey(2)));
	layout->addWidget( (keys[3] = new ArrawKey(3)));
	layout->addWidget( (keys[4] = new ArrawKey(4)));
}


Keypad::~Keypad()
{

}

void Keypad::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	QColor lightColor(Qt::lightGray);
	QColor darkColor(Qt::black);
	qreal w = parentWidget()->width()/2;
	qreal h = parentWidget()->height()/2;
	int e = 6; // edge

	keys[0]->setGeometry(w/2+w/3+e ,h/2+h/3+e, w/3-2*e,h/3-2*e);
	keys[1]->setGeometry(w/2+w/3+e ,h/2+0+e, w/3-2*e,h/3-2*e);
	keys[2]->setGeometry(w/2+0+e ,h/2+h/3+e, w/3-2*e,h/3-2*e);		
	keys[3]->setGeometry(w/2+2*w/3+e ,h/2+h/3+e, w/3-2*e,h/3-2*e);	
	keys[4]->setGeometry(w/2+1*w/3+e ,h/2+2*h/3+e, w/3-2*e,h/3-2*e);		

	setGeometry(0 ,0, w*2,2*h);

}	


