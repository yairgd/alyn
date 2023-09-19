/*
 * =====================================================================================
 *
 *       Filename:  uart.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/01/2023 09:45:59 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "uart.h"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>

#include <string.h>

/* change this to any other UART peripheral if desired */
//#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)
#define UART_DEVICE_NODE DT_CHOSEN(zephyr_console)

#define MSG_SIZE 1

/* queue to store up to 10 messages (aligned to 4-byte boundary) */
K_MSGQ_DEFINE(uart_msgq, MSG_SIZE, 512, 4);

static const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

volatile static int tx_data_idx = 0;
static uint8_t *uart_data;
///static int idx =0;

/*
 * Read characters from UART until line end is detected. Afterwards push the
 * data to the message queue.
 */
void serial_cb(const struct device *dev, void *user_data)
{
	uint8_t c;
	int len;
	
	if (!uart_irq_update(uart_dev)) {
		return;
	}

/* Verify uart_irq_tx_ready() */
	/* Note that TX IRQ may be disabled, but uart_irq_tx_ready() may
	 * still return true when ISR is called for another UART interrupt,
	 * hence additional check for i < DATA_SIZE.
	 */
	if (uart_irq_tx_ready(dev) && tx_data_idx ) {
		/* We arrive here by "tx ready" interrupt, so should always
		 * be able to put at least one byte into a FIFO. If not,
		 * well, we'll fail test.
		 */
		if ((len = uart_fifo_fill(dev,
				   (uint8_t *)uart_data, 1) > 0)) {
			tx_data_idx-=1;
			uart_data++;
//			idx+=len;
//			data_transmitted = true;
//			char_sent++f
		}

		if (tx_data_idx == 0) {
			/* If we transmitted everything, stop IRQ stream,
			 * otherwise main app might never run.
			 */
			uart_irq_tx_disable(dev);
		}
	}


	while (uart_irq_rx_ready(uart_dev)) {
		uart_fifo_read(uart_dev, &c, 1);
		k_msgq_put(&uart_msgq, &c, K_NO_WAIT);
		/* else: characters beyond buffer size are dropped */
	}
}

void uart_init(void)
{
	struct uart_config m_uart_cfg;

	m_uart_cfg.baudrate = 115200;
	m_uart_cfg.flow_ctrl = UART_CFG_FLOW_CTRL_NONE;
	m_uart_cfg.parity = UART_CFG_PARITY_NONE;
	m_uart_cfg.data_bits = UART_CFG_DATA_BITS_8;
	m_uart_cfg.stop_bits = UART_CFG_STOP_BITS_1;



	if (!device_is_ready(uart_dev)) {
		printk("UART device not found!");
		return;
	}

	uart_configure(uart_dev, &m_uart_cfg);

	/* configure interrupt and callback to receive data */
	uart_irq_callback_user_data_set(uart_dev, serial_cb, NULL);
	uart_irq_rx_enable(uart_dev);

}


int  uart_receive_one_char(uint8_t *c) {
	int e = k_msgq_get(&uart_msgq, c, K_FOREVER) ;
	return e;
}


int  uart_send(uint8_t *c, int n) {
//	uart_tx(uart_dev, c, n,100);
#if 1
	tx_data_idx = n;	
	uart_data = c;
	uart_irq_tx_enable(uart_dev);
	while (tx_data_idx);
	uart_irq_tx_disable(uart_dev);
#endif
	return 0;
}

