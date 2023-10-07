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
#include "chessboardwidget.h"
#include <QPainter>
#include <qnamespace.h>
#include <iostream>
#include "effect.h"
#include "banner.h"
#include "font.h"

#include <chrono>
#include <iostream>

using namespace std::chrono_literals;


ChessboardWidget::ChessboardWidget(QWidget *parent)
    : QWidget(parent)
{

	led_matrix_init(&led_matrix, 64,32);

	struct effect_base * e =  led_matrix_get_banner(&led_matrix);
	struct banner * b = reinterpret_cast<struct banner *> (e->object_data);
	banner_init_with_text(b,(struct rect){0,0,64,15}, font_6x13(),"This is buffer #1");
	effect_set_config (e, BANNER_CONFIG_ROTATE(100,1,2));

	struct effect_base * e1 =  led_matrix_get_banner(&led_matrix);
	struct banner * b1 = reinterpret_cast<struct banner *> (e1->object_data);
	banner_init_with_text(b1,(struct rect){15,16,30,15}, font_6x13(),"באנר מספר 2  test in hebrew");
	effect_set_config (e1, BANNER_CONFIG_ROTATE(100,1,1));
	//led_matrix_manage(&led_matrix);
	this->setUpdatesEnabled(true);	
   	 startTimer(1ms);

}


ChessboardWidget::~ChessboardWidget()
{

}

void ChessboardWidget::timerEvent(QTimerEvent *event)
{
	led_matrix_manage(&led_matrix);
	this->update();
	this->repaint();

}


void ChessboardWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // Define the chessboard square size and colors
    int squareSize = 8; //width() / 8; // Assuming an 8x8 chessboard
    QColor lightColor(Qt::white);
    QColor darkColor(Qt::black);

    //painter.setPen(QPen(Qt::black, 2));
    painter.fillRect(5,5,66*squareSize-5,34*squareSize-5,Qt::black);
    // Draw the chessboard grid
    for (int row = 1; row < 33; ++row)
    {
        for (int col = 1; col < 65; ++col)
        {
         //   if (GET_BIT_COLOR(&(led_matrix.canvas),col-1,row-1)) {
                painter.fillRect(col * squareSize+2, row * squareSize+2, squareSize-2, squareSize-2, QColor(GET_BIT_COLOR(&(led_matrix.canvas),col-1,row-1)));

		//painter.setBrush(QColor(0, 0, 255, 127));
	//	painter.drawRect(col * squareSize+2, row * squareSize+2, squareSize-2, squareSize-2);
	  //  }
           // else {
             //   painter.fillRect(col * squareSize+2, row * squareSize+2, squareSize-2, squareSize-2, darkColor);
	   // }
        }
    }
}


