#include "gd32f30x.h"

#define RFID_SPI                       SPI0
#define RFID_SPI_CLK                   RCU_SPI0
#define RFID_SPI_SCK_PIN               GPIO_PIN_5                 /* PA.05 */
#define RFID_SPI_SCK_GPIO_PORT         GPIOA                       /* GPIOA */
#define RFID_SPI_SCK_GPIO_CLK          RCU_GPIOA
#define RFID_SPI_MISO_PIN              GPIO_PIN_6                 /* PA.06 */
#define RFID_SPI_MISO_GPIO_PORT        GPIOA                       /* GPIOA */
#define RFID_SPI_MISO_GPIO_CLK         RCU_GPIOA
#define RFID_SPI_MOSI_PIN              GPIO_PIN_7                 /* PA.07 */
#define RFID_SPI_MOSI_GPIO_PORT        GPIOA                       /* GPIOA */
#define RFID_SPI_MOSI_GPIO_CLK         RCU_GPIOA
#define RFID_CS_PIN                    GPIO_PIN_4                  /* PA.04 */
#define RFID_CS_GPIO_PORT              GPIOA                       /* GPIOA */
#define RFID_CS_GPIO_CLK               RCU_GPIOA
#define RFID_DUMMY_BYTE                0x55
#define RFID_UART                        USART1
#define RFID_IRQ_PORT                    GPIOA
#define RFID_IRQ_CLK                     RCU_GPIOA
#define RFID_IRQ_PIN                     GPIO_PIN_8

#define MAX_NFC_READTRIES 10   // Amount of tries for every nfc block-scan

uint8_t FirstRun = 1;
int noDiffCount = 0;
int sensorMinutesElapse;
float lastGlucose;
float trend[16];
float glucoseReading;
uint8_t TXBuffer[40]; // transmit buffer
uint8_t RXBuffer[40]; // receive buffer
uint8_t NFCState = 0; // used to track NFC state
int count = 0;

#define ProtocolSelect 0x02
#define SendRecv 0x04

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
/**
  * @brief  Select SPI FLASH: Chip Select pin low
  */
#define RFID_CS_LOW()       gpio_bit_reset(RFID_CS_GPIO_PORT, RFID_CS_PIN)
/**
  * @brief  Deselect SPI FLASH: Chip Select pin high
  */
#define RFID_CS_HIGH()      gpio_bit_set(RFID_CS_GPIO_PORT, RFID_CS_PIN)

#define RFID_IRQ_LOW()       gpio_bit_reset(RFID_IRQ_PORT, RFID_IRQ_PIN)
/**
  * @brief  Deselect SPI FLASH: Chip Select pin high
  */
#define RFID_IRQ_HIGH()      gpio_bit_set(RFID_IRQ_PORT, RFID_IRQ_PIN)


// init
void RFID_Init(void);
void RFID_CR95HF_Reset(void);
// Tx, Rx
void RFID_CR95HF_Send(uint8_t byte, uint8_t len, ...);
bool RFID_CR95HF_Receive();
// Command
bool RFID_Hibernate_Command();
bool RFID_IDN_Command();
bool RFID_Select_ISO_IEC_14443_A_Protocol();
char* RFID_GetTagID(void);
bool RFID_set_para();
bool RFID_set_gain();

uint8_t RFID_SendByte(uint8_t byte);