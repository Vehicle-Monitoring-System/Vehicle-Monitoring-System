/*

Demo Description

Press button 1 to set the timer for reminder
Press button 2 to select the LED alarm mode


timer_mode=1: 15 second
timer_mode=2: 1/2 minute
timer_mode=3: 1 minute
timer_mode=4: 2 minute

led_mode=1: running led
led_mode=2: flash red 500 ms
led_mode=3: flash green 500 ms
led_mode=4: flash blue 500 ms
*/

#include "gd32f30x.h"
#include "gd32f30x_gpio.h"
#include "gd32f30x_misc.h"
#include "gd32f30x_pmu.h"
#include "gd32f30x_rcu.h"
#include "gd32f30x_spi.h"
#include "systick.h"
#include "GPS.h"
#include "tcp.h"
#include "gd32f30x_usart.h"
#include "gd32f30x_it.h"
#include "atc_e25.h"
#include "e24_flash.h"
#include <string.h>

uint8_t led_mode = 1;
uint8_t timer_mode = 1;
uint32_t counter = 0;
uint32_t alarm = 15;

#define MAX_RECEIVED_DATA1 1024

char received_data1[MAX_RECEIVED_DATA1];
char received_data2[MAX_RECEIVED_DATA1];
volatile uint32_t index = 0;
volatile uint8_t data_received = 0;

gps_t gh;

int main(void)
{
	// initial clock
	systick_config();
	// init LED and Button
	atc25_led_init();
	atc25_btn1_init(KEY_MODE_EXTI);
	atc25_btn2_init(KEY_MODE_EXTI);
	atc25_led_running(200);

	rcu_periph_clock_enable(RCU_GPIOD);
	gpio_init(GPIOD, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

	// Bật Quectel
	gpio_bit_reset(GPIOD, GPIO_PIN_2);
	delay_1ms(1000);
	gpio_bit_set(GPIOD, GPIO_PIN_2);

	uart3_init();
	usart1_init();
	usart2_init();
	usart0_init();

	rcu_periph_clock_enable(LED_GREEN_CLK);
	gpio_init(LED_GREEN_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_GREEN_PIN | LED_RED_PIN);
	gpio_bit_set(LED_GREEN_PORT, LED_GREEN_PIN | LED_RED_PIN); // turn off led green & red

	// Init RFID
	RFID_Init();

	delay_1ms(5000);

	tcp_init();
	char *uartdebug = "AT+QGPSCFG=\"outport\",\"uartdebug\"\r\n";
	char *gnss = "AT+QGPS=1\r\n";
	usart_string_transmit(USART2, uartdebug);
	usart_string_transmit(USART2, gnss);
	AFIO_PCF0 |= (uint32_t)0x02000000;

	gps_init(&gh);
	usart_receive_nmea(USART0, received_data1, 1024);
	gps_process(&gh, received_data1, 1024);

	while (1)
	{
		// char *ATI = "AT+CPIN?\r\n";
		// usart_string_transmit(USART2, ATI);
		// usart_string_transmit(UART3, received_data1);
		gps_init(&gh);
		delay_1ms(100);
		usart_receive_nmea(USART0, received_data1, 1024);
		// usart_string_transmit(USART1, received_data1);
		gps_process(&gh, received_data1, 1024);

		if (RFID_IDN_Command()) // reads the CR95HF ID
		{
			if (Select_ISO_IEC_14443_A_Protocol() == 1) // ISO 15693 settings
			{
				if (RFID_set_para() == 1)
				{
					char *str = RFID_GetTagID();
					tcp_assemble(&gh, str);
				}
			}
		}

		// sprintf(received_data1, "\r\n\r\n\r\n %lf", gh.latitude);
		// usart_string_transmit(UART3, received_data1);
	}
}