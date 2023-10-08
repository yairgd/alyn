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
#include <qnamespace.h>
#include <iostream>
#include "effect.h"
#include "banner.h"
#include "font.h"
#include "frame.h"

#include <chrono>
#include <iostream>

using namespace std::chrono_literals;


LedMatrixWidget::LedMatrixWidget(QWidget *parent)
    : QWidget(parent)
{

	led_matrix_init(&led_matrix, 64,32);
	
	struct effect_base * e =  led_matrix_get_banner(&led_matrix);
	struct banner * b = reinterpret_cast<struct banner *> (e->object_data);
	banner_init_with_text(b,(struct rect){2,2,60,15}, font_6x13(),"This is buffer #1");
	effect_set_config (e, BANNER_CONFIG_ROTATE(30,1,2));


	struct effect_base * e1 =  led_matrix_get_banner(&led_matrix);
	struct banner * b1 = reinterpret_cast<struct banner *> (e1->object_data);
	banner_init_with_text(b1,(struct rect){15,13,30,15}, font_6x13(),"באנר מספר 2  test in hebrew");
	effect_set_config (e1, BANNER_CONFIG_BLINK(500,1,2,500,1000) );




	struct effect_base * e2 =  led_matrix_get_frame(&led_matrix);
	struct frame * f2 = reinterpret_cast<struct frame *> (e2->object_data);
	frame_init (f2,  &(struct animate_frame ) {RGB(255,0,0), RGB(0,255,0), 10,10, (struct rect){0,0,63,31} }) ; 

	struct effect_base * e3 =  led_matrix_get_frame(&led_matrix);
	struct frame * f3 = reinterpret_cast<struct frame *> (e3->object_data);
	frame_init (f3,  &(struct animate_frame ) {RGB(255,0,255), RGB(255,255,0), 5,20, (struct rect){1,1,61,29} }) ; 





	//this->setUpdatesEnabled(true);	
   	 startTimer(1ms);



}


LedMatrixWidget::~LedMatrixWidget()
{

}

void LedMatrixWidget::timerEvent(QTimerEvent *event)
{
	led_matrix_manage(&led_matrix);
	this->update();

}


void LedMatrixWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    // Define the chessboard square size and colors
    int squareSize = 8; //width() / 8; // Assuming an 8x8 chessboard
    QColor lightColor(Qt::white);
    QColor darkColor(Qt::black);

    //painter.setPen(QPen(Qt::black, 2));
    painter.fillRect(0,0,64*squareSize-0,32*squareSize-0,Qt::black);
    // Draw the chessboard grid
    for (int row = 0; row < 32; ++row)
    {
        for (int col = 0; col < 64; ++col)
        {
                painter.fillRect(col * squareSize+2, row * squareSize+2, squareSize-2, squareSize-2, QColor(GET_BIT_COLOR(&(led_matrix.canvas),col-0,row-0)));

        }
    }
}


