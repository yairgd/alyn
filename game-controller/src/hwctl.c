/*
 * =====================================================================================
 *
 *       Filename:  hwctl.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/14/2023 03:43:23 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "hwctl.h"
#include "system_model.h"



#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>


/* size of stack area used by each thread */
#define STACKSIZE 4096

/* scheduling priority used by each thread */
#define PRIORITY 7




K_SEM_DEFINE(my_signal, 0, 1); // Define and initialize a semaphore


#define STATION_BUTTON_NODE DT_NODELABEL(station_button)

struct station_button {
	struct gpio_callback button_cb_data;
	const char *label;
	const struct gpio_dt_spec spec;
	int64_t press_time;
	int id;
	int state;
};





#ifdef CONFIG_UART_NATIVE_POSIX
#else
/* the gpios of node enable */
static struct gpio_dt_spec en_station[] = {
	DT_FOREACH_PROP_ELEM_SEP(DT_NODELABEL(enables), gpios,
			GPIO_DT_SPEC_GET_BY_IDX, (,))
};

/* the gpios of node enable */
static struct gpio_dt_spec led_red = GPIO_DT_SPEC_GET(DT_NODELABEL(station_led_r), gpios);
static struct gpio_dt_spec led_green = GPIO_DT_SPEC_GET(DT_NODELABEL(station_led_g), gpios);
static struct gpio_dt_spec led_blue = GPIO_DT_SPEC_GET(DT_NODELABEL(station_led_b), gpios);


static const struct gpio_dt_spec stop_blink_button = GPIO_DT_SPEC_GET_OR(DT_NODELABEL(stop_blink_button_dt), gpios,{0});


#define check_port(port) { \ 
	if (!device_is_ready(port)) { \
		printk("Error: GPIO device %s is not ready\n", port->name); \
			return;\
	} \
	if (!device_is_ready(port)) { \
		printk("Error: GPIO device %s is not ready\n", port->name); \
			return; \
	} \
}

#endif

#if 0
void hwctl_enable_node(int id) {
#ifdef CONFIG_UART_NATIVE_POSIX
#else
	// disable previous node
	gpio_pin_set_dt (&en_station[get_active_node()], 0);

	// enable the new node
	if (! is_blink(id)) {	
		// not blonking , just output the led value
		gpio_pin_set_dt (&en_station[id], 1);
		gpio_pin_set_dt (&led_red, get_r(id) ? 1 : 0);
		gpio_pin_set_dt (&led_green, get_g(id) ? 1 : 0);
		gpio_pin_set_dt (&led_blue, get_b(id) ? 1 : 0);
	} else {
		// station led in blink state, manage it
		if (time_elapse_since_last_toggle(id)) {
			// time to toggle led
			toggle_led(id);

			gpio_pin_set_dt (&en_station[id], 1);
			gpio_pin_set_dt (&led_red, get_r(id) & is_blink_on(id) ? 1 : 0);
			gpio_pin_set_dt (&led_green, get_g(id) & is_blink_on(id) ? 1 : 0);
			gpio_pin_set_dt (&led_blue, get_b(id) & is_blink_on(id)? 1 : 0);

		}
	}
#endif
	// save rhe new node
	set_active_node(id);	
}
#else
void hwctl_enable_node(int id) {
#ifdef CONFIG_UART_NATIVE_POSIX
#else
	// disable previous node
	gpio_pin_set_dt (&en_station[get_active_node()], 0);

	// enable the current node
	gpio_pin_set_dt (&en_station[id], 1);

	// save rhe new node
	set_active_node(id);	


	if (is_blink(id) ) {
		// turn on/off led according to blink state
		gpio_pin_set_dt (&led_red, get_r(id) & is_blink_on(id) ? 1 : 0);
		gpio_pin_set_dt (&led_green, get_g(id) & is_blink_on(id) ? 1 : 0);
		gpio_pin_set_dt (&led_blue, get_b(id) & is_blink_on(id)? 1 : 0);
	} else {
		// turn on led accorinding to its current color
		gpio_pin_set_dt (&led_red, get_r(id) );
		gpio_pin_set_dt (&led_green, get_g(id) );
		gpio_pin_set_dt (&led_blue, get_b(id) );
	}

	gpio_pin_set_dt (&led_red, get_r(id) ? 1 : 0);
	gpio_pin_set_dt (&led_green, get_g(id) ? 1 : 0);
	gpio_pin_set_dt (&led_blue, get_b(id) ? 1 : 0);

#endif
}
#endif

void hwctl_disable_all_nodes() {
#ifdef CONFIG_UART_NATIVE_POSIX
#else
	for (int i =0;i < 8; i++) {
		set_active_node(i);	
		gpio_pin_set_dt (&en_station[i], 0);		
	}

	gpio_pin_set_dt (&led_red, 0);
	gpio_pin_set_dt (&led_green, 0);
	gpio_pin_set_dt (&led_blue, 0);
#endif

}




#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
			DT_SPEC_AND_COMMA)
};

int hwctl_adc(void)
{
	int err;
	uint32_t count = 0;
	uint16_t buf;
	struct adc_sequence sequence = {
		.buffer = &buf,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(buf),
	};

	/* Configure channels individually prior to sampling. */
	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
		if (!device_is_ready(adc_channels[i].dev)) {
			printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
			return 0;
		}

		err = adc_channel_setup_dt(&adc_channels[i]);
		if (err < 0) {
			printk("Could not setup channel #%d (%d)\n", i, err);
			return 0;
		}
	}

	while (1) {
		printk("ADC reading[%u]:\n", count++);
		for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
			int32_t val_mv;

			printk("- %s, channel %d: ",
					adc_channels[i].dev->name,
					adc_channels[i].channel_id);

			(void)adc_sequence_init_dt(&adc_channels[i], &sequence);

			err = adc_read(adc_channels[i].dev, &sequence);
			if (err < 0) {
				printk("Could not read (%d)\n", err);
				continue;
			}

			/*
			 * If using differential mode, the 16 bit value
			 * in the ADC sample buffer should be a signed 2's
			 * complement value.
			 */
			if (adc_channels[i].channel_cfg.differential) {
				val_mv = (int32_t)((int16_t)buf);
			} else {
				val_mv = (int32_t)buf;
			}
			printk("%"PRId32, val_mv);
			err = adc_raw_to_millivolts_dt(&adc_channels[i],
					&val_mv);
			/* conversion to mV may not be supported, skip if not */
			if (err < 0) {
				printk(" (value in mV not available)\n");
			} else {
				printk(" = %"PRId32" mV\n", val_mv);
			}
		}

		k_sleep(K_MSEC(1000));
	}
	return 0;
}


void hwctl_start_free_run() {
	if (get_free_run() == 0) {
		set_free_run_delay(get_free_run_delay());
		set_free_run(1);		
		k_sem_give(&my_signal); 
	}
}

void hwctl_stop_free_run() {
	set_free_run(0);	
}


void hwctl_set_connected_stations() {
	for (int i= 0; i <8;i++) {
		set_connected(i,1);
	}
}


void hwctl_manage_blink() {
	static int i = 0;

	timer_manage();			
	manage_blink(i);			
	hwctl_enable_node(i);
	int value = gpio_pin_get_dt(&stop_blink_button);
	if (value < 0) {
		printk("Error %d: failed to read pin %d on %s\n", value, stop_blink_button.pin, stop_blink_button.port->name);
	} else if (value == 1) {
		set_button_state(i, value);				
		printk("station %d Pin state: %d\n", i,value);	
	}
	i++;
	i %= 8;

}

void hwctl_init ()
{

#ifdef CONFIG_UART_NATIVE_POSIX
#else


	int i = 0;

	for (i = 0;i<8;i++)
	{
		gpio_pin_configure_dt(&en_station[i], GPIO_OUTPUT);
	}

	// configure rgb output (common to all 8 stations)
	gpio_pin_configure_dt(&led_red, GPIO_OUTPUT); check_port (led_red.port); 
	gpio_pin_configure_dt(&led_green, GPIO_OUTPUT); check_port (led_green.port); 
	gpio_pin_configure_dt(&led_blue, GPIO_OUTPUT); check_port (led_blue.port); 

	// configure the station input (one button,  common to all 8 station)
	gpio_pin_configure_dt(&stop_blink_button, GPIO_INPUT); check_port (stop_blink_button.port); 

	// seeclt all connected stations
	hwctl_set_connected_stations();
#endif
}


