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

#include <qnamespace.h>

#include <iostream>
#include "effect.h"
#include "banner.h"
#include "font.h"
#include "frame.h"
#include "ledcircle.h"
#include "system_model.h"

#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

LedCircle::LedCircle(QWidget *parent, int id)
	: QWidget(parent)
{

	this->id = id; 	



}


LedCircle::~LedCircle()
{

}



void LedCircle::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	QColor lightColor(Qt::lightGray);
	QColor darkColor(Qt::black);
	qreal w = parentWidget()->width();
	qreal h = parentWidget()->height();
	qreal r1 = 13;
	qreal r2 = 13;
	int e = 6; // edge

	this->setGeometry(0,0,w,h);




	painter.fillRect(5,5,w-10,h-10,darkColor);		
	painter.fillRect(e,e,w-2*e,h-2*e,lightColor);

	painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
	painter.drawEllipse(QPointF((w)/2,(h)/2), r1,r2);		
	if (!is_connected (id) /*cb && cb->checkState() == Qt::Unchecked*/) {		
		painter.drawLine(e,e,w-e,h-e);
		painter.drawLine(w-e,e,e,h-e);
		// set the station state as unconncted 
		//set_connected(id,0);
	} else {
		// set the station state as unconncted 
		//set_connected(id,1);

		// plot led color	
		if (is_blink(id) ) {
			if (is_blink_on(id)) {
				painter.setBrush(QColor(get_rgb(id)));				
			} else {
				painter.setBrush(QColor());				
			}
		} else {
			painter.setBrush(QColor(get_rgb(id)));
		}
		painter.drawEllipse(QPointF((w)/2,(h)/2), r1*0.95,r2*0.95);				
	}

}	
void  LedCircle::mousePressEvent(QMouseEvent* event) 
{
	if (is_connected (id))
		// set the station state as unconncted 
		set_connected(id,0);
	else
		//set the station state as unconncted 
		set_connected(id,1);
}



