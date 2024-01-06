#include "atc_e25.h"
#include "gd32f30x.h"
#include "gd32f30x_gpio.h"
#include "gd32f30x_rcu.h"
#include "systick.h"
#include "gd32f30x_exti.h"
#include "string.h"

uint8_t time_elapsed1 = 0;

// init led RGB on board
void atc25_led_init(void)
{
	rcu_periph_clock_enable(LED_BLUE_CLK);
	gpio_init(LED_BLUE_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_BLUE_PIN);
	gpio_bit_set(LED_BLUE_PORT, LED_BLUE_PIN); // turn off led blue

	rcu_periph_clock_enable(LED_GREEN_CLK);
	gpio_init(LED_GREEN_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_GREEN_PIN | LED_RED_PIN);
	gpio_bit_set(LED_GREEN_PORT, LED_GREEN_PIN | LED_RED_PIN); // turn off led green & red

	rcu_periph_clock_enable(ACC_CLK);
	gpio_init(ACC_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, ACC_PIN);
}

void atc25_btn1_init(keymode_typedef_enum btn_mode)
{
	if (btn_mode != KEY_MODE_EXTI)
	{
		rcu_periph_clock_enable(BTN1_GPIO_CLK);
		gpio_init(BTN1_GPIO_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, BTN1_PIN);
	}
	else
	{
		rcu_periph_clock_enable(BTN1_GPIO_CLK);
		rcu_periph_clock_enable(RCU_AF);
		gpio_init(BTN1_GPIO_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, BTN1_PIN);

		nvic_irq_enable(BTN1_EXTI_IRQn, 1U, 0U);
		/* connect key EXTI line to key GPIO pin */
		gpio_exti_source_select(BTN1_EXTI_PORT_SOURCE, BTN1_EXTI_PIN_SOURCE);
		/* configure key EXTI line */
		exti_init(BTN1_EXTI_LINE, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
		// exti_interrupt_enable(BTN1_EXTI_LINE);
		exti_interrupt_flag_clear(BTN1_EXTI_LINE);
	}
}

void atc25_btn2_init(keymode_typedef_enum btn_mode)
{
	if (btn_mode != KEY_MODE_EXTI)
	{
		rcu_periph_clock_enable(BTN2_GPIO_CLK);
		gpio_init(BTN2_GPIO_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, BTN2_PIN);
	}
	else
	{
		rcu_periph_clock_enable(BTN2_GPIO_CLK);
		rcu_periph_clock_enable(RCU_AF);
		gpio_init(BTN2_GPIO_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, BTN2_PIN);

		nvic_irq_enable(BTN2_EXTI_IRQn, 2U, 0U);
		/* connect key EXTI line to key GPIO pin */
		gpio_exti_source_select(BTN2_EXTI_PORT_SOURCE, BTN2_EXTI_PIN_SOURCE);
		/* configure key EXTI line */
		exti_init(BTN2_EXTI_LINE, EXTI_INTERRUPT, EXTI_TRIG_BOTH);
		// exti_interrupt_enable(BTN2_EXTI_LINE);
		exti_interrupt_flag_clear(BTN2_EXTI_LINE);
	}
}

// turn on specific LED (RED/GREEN/BLUE) or All LED on the board
void atc25_led_on(ledlist_typedef_enum led_num)
{
	if (led_num == RED)
		gpio_bit_reset(LED_RED_PORT, LED_RED_PIN);
	else if (led_num == GREEN)
		gpio_bit_reset(LED_GREEN_PORT, LED_GREEN_PIN);
	else if (led_num == BLUE)
		gpio_bit_reset(LED_BLUE_PORT, LED_BLUE_PIN);
	else
	{
		gpio_bit_reset(LED_GREEN_PORT, LED_GREEN_PIN | LED_RED_PIN);
		gpio_bit_reset(LED_BLUE_PORT, LED_BLUE_PIN);
	}
}

// turn off LED
void atc25_led_off(ledlist_typedef_enum led_num)
{
	if (led_num == RED)
		gpio_bit_set(LED_RED_PORT, LED_RED_PIN);
	else if (led_num == GREEN)
		gpio_bit_set(LED_GREEN_PORT, LED_GREEN_PIN);
	else if (led_num == BLUE)
		gpio_bit_set(LED_BLUE_PORT, LED_BLUE_PIN);
	else
	{
		gpio_bit_set(LED_GREEN_PORT, LED_GREEN_PIN | LED_RED_PIN);
		gpio_bit_set(LED_BLUE_PORT, LED_BLUE_PIN);
	}
}
// LED running program
void atc25_led_running(uint32_t time_delay_ms)
{
	atc25_led_on(RED);
	delay_1ms(time_delay_ms);
	atc25_led_off(ALL);
	atc25_led_on(GREEN);
	delay_1ms(time_delay_ms);
	atc25_led_off(ALL);
	atc25_led_on(BLUE);
	delay_1ms(time_delay_ms);
	atc25_led_off(ALL);
	atc25_led_on(RED);
	delay_1ms(time_delay_ms);
	atc25_led_off(ALL);
	atc25_led_on(GREEN);
	delay_1ms(time_delay_ms);
	atc25_led_off(ALL);
	atc25_led_on(BLUE);
	delay_1ms(time_delay_ms);
	atc25_led_off(ALL);
}

void usart0_init(void)
{
	rcu_periph_clock_enable(RCU_USART0);
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	usart_enable(USART0);
	usart_word_length_set(USART0, USART_WL_8BIT);
	usart_stop_bit_set(USART0, USART_STB_1BIT);
	usart_baudrate_set(USART0, 115200);
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
	usart_parity_config(USART0, USART_PM_NONE);
	usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
	usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
}

void usart1_init(void)
{
	// Bật clock cho USART1 và GPIOA
	rcu_periph_clock_enable(RCU_USART1);
	rcu_periph_clock_enable(RCU_GPIOA);
	// Cấu hình chân PA9 cho USART1_TX và chân PA10 cho USART1_RX
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
	gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
	// Cấu hình USART1
	usart_deinit(USART1);
	usart_baudrate_set(USART1, 115200);
	usart_word_length_set(USART1, USART_WL_8BIT);
	usart_stop_bit_set(USART1, USART_STB_1BIT);
	usart_parity_config(USART1, USART_PM_NONE);
	usart_hardware_flow_cts_config(USART1, USART_CTS_DISABLE);
	usart_hardware_flow_rts_config(USART1, USART_RTS_DISABLE);
	usart_receive_config(USART1, USART_RECEIVE_ENABLE);
	usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
	usart_enable(USART1);
}

void uart3_init()
{
	rcu_periph_clock_enable(RCU_UART3);
	rcu_periph_clock_enable(RCU_GPIOC);
	gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
	usart_deinit(UART3);
	usart_baudrate_set(UART3, 115200);
	usart_word_length_set(UART3, USART_WL_8BIT);
	usart_stop_bit_set(UART3, USART_STB_1BIT);
	usart_parity_config(UART3, USART_PM_NONE);
	usart_hardware_flow_cts_config(UART3, USART_CTS_DISABLE);
	usart_hardware_flow_rts_config(UART3, USART_RTS_DISABLE);
	usart_receive_config(UART3, USART_RECEIVE_ENABLE);
	usart_transmit_config(UART3, USART_TRANSMIT_ENABLE);
	usart_enable(UART3);
}

void usart2_init()
{
	// Bật clock cho USART2 và GPIOB
	rcu_periph_clock_enable(RCU_USART2);
	rcu_periph_clock_enable(RCU_GPIOB);

	// Cấu hình chân PB10 cho USART2_TX và chân PB11 cho USART2_RX
	gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

	// Cấu hình USART2
	usart_deinit(USART2);
	usart_baudrate_set(USART2, 115200);
	usart_word_length_set(USART2, USART_WL_8BIT);
	usart_stop_bit_set(USART2, USART_STB_1BIT);
	usart_parity_config(USART2, USART_PM_NONE);
	usart_hardware_flow_cts_config(USART2, USART_CTS_DISABLE);
	usart_hardware_flow_rts_config(USART2, USART_RTS_DISABLE);
	usart_receive_config(USART2, USART_RECEIVE_ENABLE); // Vô hiệu hóa chức năng nhận dữ liệu
	usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);
	usart_enable(USART2);
}

void usart_string_transmit(uint32_t usart_periph, char *data)
{
	for (uint32_t i = 0; i < strlen(data); i++)
	{
		usart_data_transmit(usart_periph, data[i]);
		while (RESET == usart_flag_get(usart_periph, USART_FLAG_TBE))
			;
	}
}

void atc25_led_flash(ledlist_typedef_enum led_num, uint32_t time_ms)
{
	atc25_led_off(ALL);
	atc25_led_on(led_num);
	delay_1ms(time_ms);
	atc25_led_off(led_num);
	delay_1ms(time_ms);
	atc25_led_on(led_num);
	delay_1ms(time_ms);
	atc25_led_off(led_num);
	delay_1ms(time_ms);
}

char usart_receive_byte(uint32_t usart_periph)
{
	while (usart_flag_get(usart_periph, USART_FLAG_RBNE) == RESET)
		;
	return (char)usart_data_receive(usart_periph);
}

/// @brief
/// @param buffer
/// @param max_length

void usart_receive_AT(uint32_t usart_periph, char *buffer, uint32_t max_length)
{
	uint32_t index = 0;
	char received_char;

	while (index < max_length - 1)
	// while(time_elapsed1 == 0)
	{
		received_char = usart_receive_byte(usart_periph);
		if ((received_char == '\r') || (received_char == '\n') || (received_char == '\0'))
		{
			break;
		}
		buffer[index] = received_char;
		index++;
	}

	buffer[index] = '\r';
	buffer[index + 1] = '\n';
	buffer[index + 2] = '\r';
	buffer[index + 3] = '\n';
	buffer[index + 4] = '\0';
}

void timer1_init()
{
	// Khởi tạo cấu hình cho Timer 1
	rcu_periph_clock_enable(RCU_TIMER1);
	timer_deinit(TIMER1);
	timer_prescaler_config(TIMER1, TIMER_PSC_RELOAD_NOW, 7199); // Freq = 72 MHz / (7199 + 1) = 10 kHz
	timer_autoreload_value_config(TIMER1, 9999);				// T = 1s (10 kHz * 10000 = 1s)

	// Kích hoạt ngắt khi timer tràn
	timer_interrupt_enable(TIMER1, TIMER_INT_UP);

	// Đặt mức ưu tiên ngắt
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
	nvic_irq_enable(TIMER1_IRQn, 0, 0);

	timer_enable(TIMER1);
}

void TIMER1_IRQHandler(void)
{
	if (timer_flag_get(TIMER1, TIMER_FLAG_UP))
	{
		timer_flag_clear(TIMER1, TIMER_FLAG_UP);
		time_elapsed1 = 1; // Đánh dấu đã trôi qua thời gian 1 giây
	}
}

void usart_receive_nmea(uint32_t usart_periph, char *buffer, uint32_t max_length)
{
	uint32_t index = 0;
	char received_char;

	while (index < max_length - 1)
	{
		received_char = usart_receive_byte(usart_periph);
		if ((received_char == '\n') && (buffer[index - 1] == '\r') && (buffer[index - 2] == 'E') && (buffer[index - 3] == '2'))
		{
			buffer[index] = '\0';
			break;
		}
		else
		{
			buffer[index] = received_char;
			index++;
		}
	}
}

void usart_receive_string(uint32_t usart_periph, char *buffer, uint32_t max_length)
{
	uint32_t index = 0;

	char received_char;

	while (index < max_length - 1)
	{
		received_char = usart_receive_byte(usart_periph);
		if ((received_char == '\n') && (buffer[index - 1] == '\r') && (buffer[index - 2] == 'K') && (buffer[index - 3] == 'O'))
		{
			atc25_led_running(200);
			buffer[index] = '\0';
			break;
		}
		else if ((received_char == '\n') && (buffer[index - 1] == '\r') && (buffer[index - 2] == 'R') && (buffer[index - 3] == 'O'))
		{
			atc25_led_running(200);
			buffer[index] = '\0';
			break;
		}

		else
		{
			buffer[index] = received_char;
			index++;
		}
	}
}