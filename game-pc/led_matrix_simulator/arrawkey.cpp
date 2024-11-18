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
	switch (id) {
		case 4: img = QPixmap(":/resource/down_key.png");break;
		case 1: img = QPixmap(":/resource/up_key.png");break;
		case 2: img = QPixmap(":/resource/left_key.png");break;
		case 3: img = QPixmap(":/resource/right_key.png");break;
		case 0: img = QPixmap(":/resource/ok_key.png");break;

	};

}


ArrawKey::~ArrawKey()
{

}



void ArrawKey::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	QColor lightColor(Qt::lightGray);
	qreal w = parentWidget()->width()/2;
	qreal h = parentWidget()->height()/2;


	//https://stackoverflow.com/questions/36894246/qpainterdrawpixmap-doesnt-look-good-and-has-low-quality
//	painter.drawPixmap(QRect(50, 0, w/4, w/4), img);
//	painter.setRenderHint(QPainter::SmoothPixmapTransform);
//	painter.drawPixmap(QRect(0, 0, w/4, h/4), img);
	img = img.scaled(w/4, h/4, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	painter.drawPixmap(0, 0, img);
}	

void  ArrawKey::mousePressEvent(QMouseEvent* event) 
{
	 start = std::chrono::high_resolution_clock::now();
}


void  ArrawKey::mouseReleaseEvent(QMouseEvent* event) 
{
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = finish - start;
	if ( elapsed.count() < 500) {
		set_key(id, 1);
	} else {
		set_long_key(id, 1);		
		
	}
}




