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
#include "canvas.h"
#include "banner.h"
#include "font.h"
#include "frame.h"
#include "threadedworker.h"

#include "led_matrix.h"

#include <chrono>
#include <iostream>
#include <cstdint>

using namespace std::chrono_literals;



LedMatrixWidget::LedMatrixWidget(QWidget *parent, std::array<LedCircle *, 8>  arr)
	: QWidget(parent)
{

	QMetaObject::invokeMethod(&worker, "doWork", Qt::QueuedConnection);
	QObject::connect(&worker, &ThreadedWorker::updateLedMatrix, [this]() {
		this->update();
		for ( auto * led : led_arr) 
			led->update();

	});
	led_arr = arr;

#if 0	
	struct effect_base * e =  led_matrix_get_banner(led_matrix);
	struct banner * b = reinterpret_cast<struct banner *> (e->object_data);
	banner_init_with_text(b,(struct rect){2,2,60,15}, font_6x13(),"This is buffer #1");
	effect_set_config (e, BANNER_CONFIG_ROTATE(30,1,2));


	struct effect_base * e1 =  led_matrix_get_banner(led_matrix);
	struct banner * b1 = reinterpret_cast<struct banner *> (e1->object_data);
	banner_init_with_text(b1,(struct rect){15,13,30,15}, font_6x13(),"באנר מספר 2  test in hebrew");
	effect_set_config (e1, BANNER_CONFIG_BLINK(500,1,2,500,1000) );



	struct effect_base * e2 =  led_matrix_get_frame(led_matrix);
	struct frame * f2 = reinterpret_cast<struct frame *> (e2->object_data);
	effect_set_config (e2,  FRAME_CONFIG_TYPE_1(RGB(255,0,0), RGB(0,255,0), 10,10, RECT(0,0,63,31)));


	struct effect_base * e3 =  led_matrix_get_frame(led_matrix);
	struct frame * f3 = reinterpret_cast<struct frame *> (e3->object_data);
	effect_set_config (e3,  FRAME_CONFIG_TYPE_1(RGB(255,0,255), RGB(255,255,0), 5,20, RECT(1,1,61,29) ));
#endif





	//this->setUpdatesEnabled(true);	
	//startTimer(50ms);



}


LedMatrixWidget::~LedMatrixWidget()
{

}
#if 0
void LedMatrixWidget::timerEvent(QTimerEvent *event)
{
	//led_matrix_manage(led_matrix);
	this->update();

}
#endif

#if 0
void LedMatrixWidget::updateLedMatrix()
{
	this->update();
	for ( auto * led : led_arr) 
		led->update();
}

#endif


//static inline pixel * GET_POINTER_TO_PIXEL1(struct canvas * c,int w,int h) {
//	return ((struct pixel*)&c->buffer[sizeof (struct pixel) * (w) + sizeof (struct pixel) * (h) * c->width]);
//}
void LedMatrixWidget::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	led_matrix_merge(led_matrix_get());	
	struct channel * channel = led_matrix_get_channel(led_matrix_get(),  3 );    	


	// Define the chessboard square size and colors
	int squareSize = parentWidget()->width() / 64; 

	// define geometry of the display in its father window
	setGeometry(0, (parentWidget()->height() -  32*squareSize)/2 ,parentWidget()->width(),parentWidget()->height());


	
	QColor lightColor(Qt::white);
	QColor darkColor(Qt::black);

	//painter.setPen(QPen(Qt::black, 2));
	painter.fillRect(0,0,64*squareSize-0,32*squareSize-0,Qt::black);
	
	
	// draw pixels 
	for (int row = 0; row < 32; ++row)
	{
		for (int col = 0; col < 64; ++col)
		{	
			struct pixel * pix0 = GET_POINTER_TO_PIXEL(&(channel->canvas),col,row  );
			
		//	painter.fillRect(col * squareSize+2, row * squareSize+2, squareSize-2, squareSize-2, QColor(GET_BIT_COLOR(&(channel->canvas),col,row)));
			painter.fillRect(col * squareSize+2, row * squareSize+2, squareSize-2, squareSize-2, QColor( *((std::uint32_t*)pix0)));

		}
	}
}

