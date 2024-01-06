#include "gd32f30x.h"

#define sFLASH_SPI                       SPI0
#define sFLASH_SPI_CLK                   RCU_SPI0
#define sFLASH_SPI_SCK_PIN               GPIO_PIN_5                 /* PA.05 */
#define sFLASH_SPI_SCK_GPIO_PORT         GPIOA                       /* GPIOA */
#define sFLASH_SPI_SCK_GPIO_CLK          RCU_GPIOA
#define sFLASH_SPI_MISO_PIN              GPIO_PIN_6                 /* PA.06 */
#define sFLASH_SPI_MISO_GPIO_PORT        GPIOA                       /* GPIOA */
#define sFLASH_SPI_MISO_GPIO_CLK         RCU_GPIOA
#define sFLASH_SPI_MOSI_PIN              GPIO_PIN_7                 /* PA.07 */
#define sFLASH_SPI_MOSI_GPIO_PORT        GPIOA                       /* GPIOA */
#define sFLASH_SPI_MOSI_GPIO_CLK         RCU_GPIOA
#define sFLASH_CS_PIN                    GPIO_PIN_4                  /* PA.04 */
#define sFLASH_CS_GPIO_PORT              GPIOA                       /* GPIOA */
#define sFLASH_CS_GPIO_CLK               RCU_GPIOA


#define sFLASH_DUMMY_BYTE             0x55

/**
  * @ SPI NOR CMD
  */ 

#define sFLASH_CMD_NOR_WRITE          0x02  /*!< Write to Memory instruction */
#define sFLASH_CMD_NOR_WRSR           0x01  /*!< Write Status Register instruction */
#define sFLASH_CMD_NOR_WREN           0x06  /*!< Write enable instruction */
#define sFLASH_CMD_NOR_WDIS           0x04   /*!< Write disable instruction */
#define sFLASH_CMD_NOR_READ           0x03  /*!< Read from Memory instruction */
#define sFLASH_CMD_NOR_RDSR           0x05  /*!< Read Status Register instruction  */
#define sFLASH_CMD_NOR_MISR           0x35  /*!< Read Status Register instruction Midde Byte */
#define sFLASH_CMD_NOR_HISR           0x15  /*!< Read Status Register instruction Hige Byte */
#define sFLASH_CMD_NOR_RDID           0x9F  /*!< Read identification */
#define sFLASH_CMD_NOR_SE             0x20  /*!< Sector Erase instruction */
#define sFLASH_CMD_NOR_BE             0xD8  /*!< Block Erase instruction */
#define sFLASH_CMD_NOR_CE             0xC7  /*!< Chip Erase instruction */
#define sFLASH_WIP_FLAG          			0x01  /*!< Write In Progress (WIP) flag */

/**
  * @brief  Select SPI FLASH: Chip Select pin low
  */
#define sFLASH_CS_LOW()       gpio_bit_reset(sFLASH_CS_GPIO_PORT, sFLASH_CS_PIN)
/**
  * @brief  Deselect SPI FLASH: Chip Select pin high
  */
#define sFLASH_CS_HIGH()      gpio_bit_set(sFLASH_CS_GPIO_PORT, sFLASH_CS_PIN)
 
void sFLASH_WriteEnable(void);
void sFLASH_WriteDisable(void);

/**
  * @ SPI Nor function
  */ 

void sFLASH_NOR_WaitForWriteEnd(void);
void sFLASH_NOR_ChipErase(void);
void sFLASH_NOR_SectorErase(uint32_t SectorAddr);
void sFLASH_NOR_BlockErase(uint32_t BlockAddr);
void sFLASH_NOR_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void sFLASH_NOR_Pageprogram(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_NOR_WriteLowSR( uint8_t uldata );
void sFLASH_NOR_WriteMidSR( uint8_t uldata );
void sFLASH_NOR_WriteHiSR( uint8_t uldata );
uint8_t sFLASH_NOR_ReadLowSR( void );
uint8_t sFLASH_NOR_ReadMidSR( void );
uint8_t sFLASH_NOR_ReadHiSR( void );
uint8_t NandGetFeature(uint8_t Add);
uint32_t sFLASH_NOR_ReadID( void );
void sFLASH_NOR_WriteAllSR( uint32_t uldata);

void sFLASH_WaitForWriteEnd(void);

void sFLASH_Hardware_Init(void);
uint8_t sFLASH_ReadByte(void);
uint8_t sFLASH_SendByte(uint8_t byte);
// __SFLASH_SPI_H__