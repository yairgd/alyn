/*
 * =====================================================================================
 *
 *       Filename:  chessboardwidget.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/06/2023 04:58:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
// chessboardwidget.cpp
#include "ledmatrix.h"
#include <QPainter>
#include <QDebug>

#include <iterator>
#include <qnamespace.h>

#include <iostream>
#include "effect.h"
#include "banner.h"
#include "font.h"
#include "frame.h"
#include "arrawkey.h"
#include "system_model.h"

#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

ArrawKey::ArrawKey(int id, QWidget *parent)
	: QWidget(parent)
{

	this->id = id; 
	// make the led connected by default



}


ArrawKey::~ArrawKey()
{

}



void ArrawKey::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	QColor lightColor(Qt::lightGray);
	QColor darkColor(Qt::black);
	qreal w = parentWidget()->width()/2;
	qreal h = parentWidget()->height()/2;
	int e = 6; // edge


	switch (id) {
		case 0:
			setGeometry(w/2+w/3+e ,h/2+h/3+e, w/3-2*e,h/3-2*e);
			break;
		case 1:
			setGeometry(w/2+w/3+e ,h/2+0+e, w/3-2*e,h/3-2*e);
			break;
		case 2:
			setGeometry(w/2+0+e ,h/2+h/3+e, w/3-2*e,h/3-2*e);			
			break;
		case 3:
			setGeometry(w/2+2*w/3+e ,h/2+h/3+e, w/3-2*e,h/3-2*e);			
			break;
		case 4:
			setGeometry(w/2+1*w/3+e ,h/2+2*h/3+e, w/3-2*e,h/3-2*e);			
			break;
			

	}
	painter.fillRect(0,0,w/3,h/3,QColor(Qt::red));		


QPixmap img("/home/yair/linux_on_stm32_mcu/buildroot/docs/website/images/tux-flat.png");
 // painter.drawPixmap(QRect(50, 0, 50, 50), img);
//  painter.setRenderHint(QPainter::SmoothPixmapTransform);
  painter.drawPixmap(QRect(0, 0, w/3, h/3), img);
  img = img.scaled(w/3, h/3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  painter.drawPixmap(100, 0, img);	
}	

void  ArrawKey::mousePressEvent(QMouseEvent* event) 
{
	 set_key(id, 1); 		
	// std::cout<<"press key "<<id<<std::endl;
}


void  ArrawKey::mouseReleaseEvent(QMouseEvent* event) 
{
	// set_key(id, 0); 		
	// std::cout<<"release key "<<id<<std::endl;
}




