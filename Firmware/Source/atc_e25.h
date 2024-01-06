// this header file define ATC-Board 25 hardware

#include "gd32f30x.h"

typedef enum
{
    KEY_MODE_GPIO = 0,
    KEY_MODE_EXTI = 1
} keymode_typedef_enum;

typedef enum
{
    RED = 0,
    GREEN = 1,
    BLUE = 2,
    ALL = 3
} ledlist_typedef_enum;


// define user RGB LED on board
#define LED_GREEN_PORT GPIOC
#define LED_GREEN_PIN GPIO_PIN_0
#define LED_GREEN_CLK RCU_GPIOC

#define LED_MEMORY_PORT GPIOC
#define LED_MEMORY_PIN GPIO_PIN_0
#define LED_MEMORY_CLK RCU_GPIOC

#define LED_BLUE_PORT GPIOC
#define LED_BLUE_PIN GPIO_PIN_1
#define LED_BLUE_CLK RCU_GPIOC

#define LED_RED_PORT GPIOC
#define LED_RED_PIN GPIO_PIN_2
#define LED_RED_CLK RCU_GPIOC

#define ACC_PORT    GPIOB
#define ACC_PIN     GPIO_PIN_15
#define ACC_CLK     RCU_GPIOB

// BTN1(PA15)
#define BTN1_PIN GPIO_PIN_15
#define BTN1_GPIO_PORT GPIOA
#define BTN1_GPIO_CLK RCU_GPIOA
#define BTN1_EXTI_LINE EXTI_15
#define BTN1_EXTI_PORT_SOURCE GPIO_PORT_SOURCE_GPIOA
#define BTN1_EXTI_PIN_SOURCE GPIO_PIN_SOURCE_15
#define BTN1_EXTI_IRQn EXTI10_15_IRQn

// BTN2 (PC12)
#define BTN2_PIN GPIO_PIN_12
#define BTN2_GPIO_PORT GPIOC
#define BTN2_GPIO_CLK RCU_GPIOC
#define BTN2_EXTI_LINE EXTI_12
#define BTN2_EXTI_PORT_SOURCE GPIO_PORT_SOURCE_GPIOC
#define BTN2_EXTI_PIN_SOURCE GPIO_PIN_SOURCE_12
#define BTN2_EXTI_IRQn EXTI10_15_IRQn


void atc25_led_init(void);
void atc25_led_on(ledlist_typedef_enum led_num);
void atc25_led_off(ledlist_typedef_enum led_num);
void atc25_led_running(uint32_t time_delay_ms);
void atc25_btn1_init(keymode_typedef_enum btn_mode);
void atc25_btn2_init(keymode_typedef_enum btn_mode);

// USART
void usart0_init();
void uart3_init();
void usart1_init();
void usart2_init();

void atc25_led_flash(ledlist_typedef_enum led_num, uint32_t time_ms);

// USART transmit
void usart_string_transmit(uint32_t usart_periph, char *data);
// USART Receive
char usart_receive_byte(uint32_t usart_periph);
void usart_receive_string(uint32_t usart_periph, char *buffer, uint32_t max_length);
void usart_receive_nmea(uint32_t usart_periph, char *buffer, uint32_t max_length);
void usart_receive_AT(uint32_t usart_periph, char *buffer, uint32_t max_length);
// Enable Timer
void timer1_init();