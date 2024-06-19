/*
 * =====================================================================================
 *
 *       Filename:  gpio.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/19/2024 08:57:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#define GPIO_NODE DT_NODELABEL(gpioa)

#define BUTTON_UP_NODE DT_NODELABEL(user_button_up)
#define BUTTON_DOWN_NODE DT_NODELABEL(user_button_down)
#define BUTTON_LEFT_NODE DT_NODELABEL(user_button_left)
#define BUTTON_RIGHT_NODE DT_NODELABEL(user_button_right)
#define BUTTON_MIDDLE_NODE DT_NODELABEL(user_button_middle)
#define BUTTON_STB_NODE DT_NODELABEL(station_button)

#define GET_GPIO_PIN(node) DT_GPIO_PIN(node, gpios)
#define GET_GPIO_FLAGS(node) (GPIO_INPUT | DT_GPIO_FLAGS(node, gpios)  | GPIO_INT_EDGE_BOTH)

struct button {
	const char *label;
	const struct gpio_dt_spec spec;
	int64_t press_time;
	int id;
};

#define BUTTON_SPEC(node,id_) { .label = DT_LABEL(node), .spec = GPIO_DT_SPEC_GET_OR(node, gpios, {0}), .press_time = 0,.id=(id_) }

static struct button buttons[] = {
	BUTTON_SPEC(BUTTON_MIDDLE_NODE,1<<0),	
	BUTTON_SPEC(BUTTON_UP_NODE,    1<<1),		
	BUTTON_SPEC(BUTTON_LEFT_NODE,  1<<2),
	BUTTON_SPEC(BUTTON_RIGHT_NODE, 1<<3),	
	BUTTON_SPEC(BUTTON_DOWN_NODE,  1<<4),
	BUTTON_SPEC(BUTTON_STB_NODE,   1<<5),
};

static void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	int64_t now = k_uptime_get();
	for (int i = 0; i < ARRAY_SIZE(buttons); i++) {
		if (pins & BIT(buttons[i].spec.pin)) {
			int val = gpio_pin_get(buttons[i].spec.port, buttons[i].spec.pin);
			if (val > 0) {
				buttons[i].press_time = now;
			} else {
				int64_t duration = now - buttons[i].press_time;
				if (duration >= 2000) {
					printk("Button %s (GPIO %d) long press (%lld ms)\n", buttons[i].label, buttons[i].spec.pin, duration);
				} else {
					printk("Button %s (GPIO %d) short press (%lld ms)\n", buttons[i].label, buttons[i].spec.pin, duration);
				}
			}
		}
	}
}

static struct gpio_callback button_cb_data;

void gpio_init(void)
{
	const struct device *gpio_dev;
	int ret;

	gpio_dev = DEVICE_DT_GET(GPIO_NODE);
	if (!device_is_ready(gpio_dev)) {
		printk("Error: GPIO device %s is not ready\n", gpio_dev->name);
		return;
	}

	for (int i = 0; i < ARRAY_SIZE(buttons); i++) {
		if (!device_is_ready(buttons[i].spec.port)) {
			printk("Error: Button %s GPIO port %s is not ready\n", buttons[i].label, buttons[i].spec.port->name);
			return;
		}

		ret = gpio_pin_configure_dt(&buttons[i].spec, GET_GPIO_FLAGS(buttons[i].spec));
		if (ret != 0) {
			printk("Error %d: failed to configure pin %d for %s\n", ret, buttons[i].spec.pin, buttons[i].label);
			return;
		}

		ret = gpio_pin_interrupt_configure_dt(&buttons[i].spec, GPIO_INT_EDGE_BOTH);
		if (ret != 0) {
			printk("Error %d: failed to configure interrupt for pin %d (%s)\n", ret, buttons[i].spec.pin, buttons[i].label);
			return;
		}
	}

	gpio_init_callback(&button_cb_data, button_pressed, BIT(buttons[0].spec.pin) |
			BIT(buttons[1].spec.pin) |
			BIT(buttons[2].spec.pin) |
			BIT(buttons[3].spec.pin) |
			BIT(buttons[4].spec.pin) |
			BIT(buttons[5].spec.pin));

	gpio_add_callback(gpio_dev, &button_cb_data);


}

