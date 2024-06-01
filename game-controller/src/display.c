/*
 * =====================================================================================
 *
 *       Filename:  display.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/04/24 07:25:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#include "game.h"

#include "display.h"
#include "led_matrix.h"
#include "canvas.h"



#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/dma.h>

/* size of stack area used by each thread */
#define STACKSIZE 4096

/* scheduling priority used by each thread */
#define PRIORITY 7


/* Define the SPI device */
#define SPI_DEVICE_NAME DT_LABEL(DT_ALIAS(spi_screen))

/* Define the DMA channel */
#define DMA_DEVICE_NAME DT_LABEL(DT_ALIAS(dma_screen))

/* DMA configuration */
#define DMA_CHANNEL 1

/* Buffer size */
#define BUFFER_SIZE 256

/* display size setup */
#define PIXELS_PER_ROW 64
#define NUMBER_OF_ROWS 16

static const struct device *spi_dev;
static const struct device *dma_dev;
//static uint8_t tx_buffer[BUFFER_SIZE]={1,2,3,4,5,6,7,8,9};
//static uint8_t rx_buffer[BUFFER_SIZE];
static struct k_sem spi_sem;

static struct gpio_dt_spec line_a = GPIO_DT_SPEC_GET(DT_NODELABEL(line_a), gpios);
static struct gpio_dt_spec line_b = GPIO_DT_SPEC_GET(DT_NODELABEL(line_b), gpios);
static struct gpio_dt_spec line_c = GPIO_DT_SPEC_GET(DT_NODELABEL(line_c), gpios);
static struct gpio_dt_spec line_d = GPIO_DT_SPEC_GET(DT_NODELABEL(line_d), gpios);

static struct gpio_dt_spec line_output_en = GPIO_DT_SPEC_GET(DT_NODELABEL(line_output_en), gpios);

//static struct spi_buf_set rx_buf;


static struct spi_config  spi_cfg = {
	.frequency = 10000000,
	.operation = SPI_OP_MODE_MASTER | SPI_WORD_SET(8),
	.slave = 0,
	.cs = NULL
};

static char pixel_buffer[PIXELS_PER_ROW * 2 * 3 / 8];

static struct spi_buf 	tx_buf = {
	.buf = pixel_buffer,
	.len = sizeof(pixel_buffer)
};

static struct spi_buf_set  tx_rx_bufs = {
	.buffers = &tx_buf,
	.count = 1
};


static void _spi_dma_callback(const struct device *dev, void *user_data, uint32_t channel, int status)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(user_data);
	ARG_UNUSED(channel);

	if (status != 0) {
		printk("SPI DMA Transfer failed with status %d\n", status);
		return;
	}

	/* SPI DMA transfer completed */
	k_sem_give(&spi_sem);
}

static void _spi_write() {
}

static void _spi_init()  {



#if 0	  
	spi_buf rx_buf = {
		.buf = rx_buffer,
		.len = PIXELS_PER_ROW * 2 * 3
	};
#endif


	spi_dev = device_get_binding(SPI_DEVICE_NAME);
	if (!spi_dev) {
		printk("Cannot find SPI device!\n");
	}
}


/**
 * Created  05/04/24
 * @brief   mux selection the line to active 0-16
 * @param   
 * @return  
 */
static void _select_line(int n) {
	if (n&0x1) gpio_pin_set_dt (&line_a, 1); else gpio_pin_set_dt (&line_a, 0); 
	if (n&0x2) gpio_pin_set_dt (&line_b, 1); else gpio_pin_set_dt (&line_b, 0); 
	if (n&0x4) gpio_pin_set_dt (&line_c, 1); else gpio_pin_set_dt (&line_c, 0); 
	if (n&0x8) gpio_pin_set_dt (&line_d, 1); else gpio_pin_set_dt (&line_d, 0); 	
}


static void _output_enable(int v)
{
	gpio_pin_set_dt (&line_output_en, v&0x1); 
}
void display__init() 
{
	gpio_pin_configure_dt(&line_a, GPIO_OUTPUT);
	gpio_pin_configure_dt(&line_b, GPIO_OUTPUT);
	gpio_pin_configure_dt(&line_c, GPIO_OUTPUT);
	gpio_pin_configure_dt(&line_d, GPIO_OUTPUT);

	gpio_pin_configure_dt(&line_output_en, GPIO_OUTPUT);

	_output_enable(1);
	_spi_init();
	_select_line(3);

}

struct pixel {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t o;
};


void SET_PIXEL_BIT(char * buf, int n, int v) {
	buf[n/8] |=  (v << (7 - (n)%8));
}




int line;
// Callback function for the timer
void display_callback(struct k_timer *dummy) {
	led_matrix_merge(led_matrix_get());		
	struct channel * channel = led_matrix_get_channel(led_matrix_get(),  0 );   

	_output_enable(1);
	_select_line (line);
	memset(pixel_buffer,0,sizeof (pixel_buffer));
	for (int col = 0; col < PIXELS_PER_ROW*3; col+=3) {			 
		struct pixel * pix0 = GET_POINTER_TO_PIXEL(&(channel->canvas),col / 3,(NUMBER_OF_ROWS - line) + NUMBER_OF_ROWS * 0 );
		SET_PIXEL_BIT(pixel_buffer,   0 * PIXELS_PER_ROW + col/3 + 0, pix0->b > 0);
		SET_PIXEL_BIT(pixel_buffer,   1 * PIXELS_PER_ROW + col/3 + 0, pix0->g > 0);
		SET_PIXEL_BIT(pixel_buffer,   2 * PIXELS_PER_ROW + col/3 + 0, pix0->r > 0);

		struct pixel * pix1 = GET_POINTER_TO_PIXEL(&(channel->canvas),col / 3,(NUMBER_OF_ROWS - line) +  NUMBER_OF_ROWS * 1);
		SET_PIXEL_BIT(pixel_buffer,  0 * PIXELS_PER_ROW + 3 * PIXELS_PER_ROW + col/3  + 0, pix1->b > 0);
		SET_PIXEL_BIT(pixel_buffer,  1 * PIXELS_PER_ROW + 3 * PIXELS_PER_ROW + col/3  + 0, pix1->g > 0);
		SET_PIXEL_BIT(pixel_buffer,  2 * PIXELS_PER_ROW + 3 * PIXELS_PER_ROW + col/3  + 0, pix1->r > 0);
	}

	for (int i=0;i<48;i++) {
		pixel_buffer[i] = 0xff;
	}
	if (spi_write(spi_dev, &spi_cfg, &tx_rx_bufs) != 0) {
		printk("SPI DMA transfer failed!\n");
	}

	_output_enable(0);

	line++;
	line %= NUMBER_OF_ROWS;

}

// Callback function for when the timer stops
void display_timer_callback(struct k_timer *dummy) {
	printk("Timer stopped\n");
}

// Define and initialize the timer
K_TIMER_DEFINE(display, display_callback, display_timer_callback);

void display_thread (void *p1,void *p2, void *p3)
{
	led_matrix_init(led_matrix_get(), 64,32);
	game_init();
	display__init();
#if 1
	line = 0;
	k_timer_start(&display, K_USEC(0), K_USEC(500));  // Start after 1 second, then every 1 second


	return;
#else
	// (uint32_t)sys_clock_tick_get()
	while (1) {
		led_matrix_merge(led_matrix_get());		
		struct channel * channel = led_matrix_get_channel(led_matrix_get(),  0 );   

		for (int line = NUMBER_OF_ROWS - 1; line >= 0 ; line--) {
			_output_enable(1);
			_select_line (line);
			memset(pixel_buffer,0,sizeof (pixel_buffer));
			for (int col = 0; col < PIXELS_PER_ROW*3; col+=3) {			 
				struct pixel * pix0 = GET_POINTER_TO_PIXEL(&(channel->canvas),col / 3,(NUMBER_OF_ROWS - line) + NUMBER_OF_ROWS * 0 );
				SET_PIXEL_BIT(pixel_buffer,   0 * PIXELS_PER_ROW + col/3 + 0, pix0->b > 0);
				SET_PIXEL_BIT(pixel_buffer,   1 * PIXELS_PER_ROW + col/3 + 0, pix0->g > 0);
				SET_PIXEL_BIT(pixel_buffer,   2 * PIXELS_PER_ROW + col/3 + 0, pix0->r > 0);

				struct pixel * pix1 = GET_POINTER_TO_PIXEL(&(channel->canvas),col / 3,(NUMBER_OF_ROWS - line) +  NUMBER_OF_ROWS * 1);
				SET_PIXEL_BIT(pixel_buffer,  0 * PIXELS_PER_ROW + 3 * PIXELS_PER_ROW + col/3  + 0, pix1->b > 0);
				SET_PIXEL_BIT(pixel_buffer,  1 * PIXELS_PER_ROW + 3 * PIXELS_PER_ROW + col/3  + 0, pix1->g > 0);
				SET_PIXEL_BIT(pixel_buffer,  2 * PIXELS_PER_ROW + 3 * PIXELS_PER_ROW + col/3  + 0, pix1->r > 0);
			}


			if (spi_write(spi_dev, &spi_cfg, &tx_rx_bufs) != 0) {
				printk("SPI DMA transfer failed!\n");
			}

			_output_enable(0);
			k_sleep(K_USEC(1000));			

		}
	}
#endif
}


K_THREAD_DEFINE(display_thread_id, STACKSIZE, display_thread, NULL , NULL, NULL, PRIORITY, 0, 0);

