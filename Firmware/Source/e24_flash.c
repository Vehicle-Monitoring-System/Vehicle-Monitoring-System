#include "gd32f30x.h"
#include "e24_flash.h"
#include "gd32f30x_gpio.h"

void sFLASH_Hardware_Init(void)
{
  // clock
  rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_SPI0);
  rcu_periph_clock_enable(RCU_AF);
  // pins
  gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, sFLASH_SPI_SCK_PIN | sFLASH_SPI_MOSI_PIN);
  gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, sFLASH_SPI_MISO_PIN);
  gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, sFLASH_CS_PIN);

  sFLASH_CS_HIGH();

  spi_parameter_struct spi_init_struct;
  spi_nss_output_enable(sFLASH_SPI);

  /* SPI0 parameter config */
  spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
  spi_init_struct.device_mode = SPI_MASTER;
  spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;
  spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
  spi_init_struct.nss = SPI_NSS_SOFT;
  spi_init_struct.prescale = SPI_PSC_256;
  spi_init_struct.endian = SPI_ENDIAN_MSB;
  spi_init(SPI0, &spi_init_struct);

  spi_enable(sFLASH_SPI);
}

uint8_t sFLASH_ReadByte(void)
{
  return (sFLASH_SendByte(sFLASH_DUMMY_BYTE));
}

uint8_t sFLASH_SendByte(uint8_t byte)
{
  /*!< Loop while DR register in not emplty */
  while (spi_i2s_flag_get(sFLASH_SPI, SPI_FLAG_TBE) == RESET)
    ;

  /*!< Send byte through the SPI1 peripheral */
  spi_i2s_data_transmit(sFLASH_SPI, byte);

  /*!< Wait to receive a byte */
  while (spi_i2s_flag_get(sFLASH_SPI, SPI_FLAG_RBNE) == RESET)
    ;

  /*!< Return the byte read from the SPI bus */
  return spi_i2s_data_receive(sFLASH_SPI);
}
// __SFLASH_SPI_H__

uint32_t sFLASH_NOR_ReadID(void)
{
  uint8_t data1;
  uint8_t data2;
  uint8_t data3;
  uint32_t data;
  sFLASH_CS_LOW();
  sFLASH_SendByte(sFLASH_CMD_NOR_RDID);
  data1 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
  data2 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
  data3 = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
  sFLASH_CS_HIGH();
  data = (data1 << 16) | (data2 << 8) | data3;
  return data;
}

void sFLASH_NOR_ChipErase()
{
  /*!< Send write enable instruction */
  sFLASH_WriteEnable();

  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();
  /*!< Send Sector Erase instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_CE);

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

  /*!< Wait the end of Flash writing */
  sFLASH_NOR_WaitForWriteEnd();
}

void sFLASH_NOR_SectorErase(uint32_t SectorAddr)
{
  /*!< Send write enable instruction */
  sFLASH_WriteEnable();

  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();
  /*!< Send Sector Erase instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_SE);
  /*!< Send SectorAddr high nibble address byte */
  sFLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  sFLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  sFLASH_SendByte(SectorAddr & 0xFF);
  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

  /*!< Wait the end of Flash writing */
  sFLASH_NOR_WaitForWriteEnd();
}
void sFLASH_NOR_BlockErase(uint32_t SectorAddr)
{
  /*!< Send write enable instruction */
  sFLASH_WriteEnable();

  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();
  /*!< Send Sector Erase instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_BE);
  /*!< Send SectorAddr high nibble address byte */
  sFLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  sFLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  sFLASH_SendByte(SectorAddr & 0xFF);
  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

  /*!< Wait the end of Flash writing */
  sFLASH_NOR_WaitForWriteEnd();
}
void sFLASH_WriteEnable(void)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Write Enable" instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_WREN);

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();
}
void sFLASH_WriteDisable(void)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Write Enable" instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_WDIS);

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();
}
/**
 * @brief  Polls the status of the Write In Progress (WIP) flag in the FLASH's
 *         status register and loop until write opertaion has completed.
 * @param  None
 * @retval None
 */
void sFLASH_WaitForWriteEnd(void)
{
  uint8_t flashstatus = 0;

  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Read Status Register" instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_RDSR);

  /*!< Loop as long as the memory is busy with a write cycle */
  do
  {
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    flashstatus = sFLASH_SendByte(sFLASH_DUMMY_BYTE);

  } while ((flashstatus & sFLASH_WIP_FLAG) == SET); /* Write in progress */

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();
}

void sFLASH_NOR_WaitForWriteEnd(void)
{
  uint8_t flashstatus = 0;

  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Read Status Register" instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_RDSR);

  /*!< Loop as long as the memory is busy with a write cycle */
  do
  {
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    flashstatus = sFLASH_SendByte(sFLASH_CMD_NOR_RDSR);

  } while ((flashstatus & sFLASH_WIP_FLAG) == SET); /* Write in progress */

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();
}
void sFLASH_NOR_Pageprogram(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  /*!< Enable the write access to the FLASH */
  sFLASH_WriteEnable();

  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();
  /*!< Send "Write to Memory " instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_WRITE);
  /*!< Send WriteAddr high nibble address byte to write to */
  sFLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*!< Send WriteAddr medium nibble address byte to write to */
  sFLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*!< Send WriteAddr low nibble address byte to write to */
  sFLASH_SendByte(WriteAddr & 0xFF);

  /*!< while there is data to be written on the FLASH */
  while (NumByteToWrite--)
  {
    /*!< Send the current byte */
    sFLASH_SendByte(*pBuffer);
    /*!< Point on the next byte to be written */
    pBuffer++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();

  /*!< Wait the end of Flash writing */
  sFLASH_NOR_WaitForWriteEnd();
}

void sFLASH_NOR_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_CS_LOW();

  /*!< Send "Read from Memory " instruction */
  sFLASH_SendByte(sFLASH_CMD_NOR_READ);

  /*!< Send ReadAddr high nibble address byte to read from */
  sFLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte to read from */
  sFLASH_SendByte((ReadAddr & 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte to read from */
  sFLASH_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) /*!< while there is data to be read */
  {
    /*!< Read a byte from the FLASH */
    *pBuffer = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
    /*!< Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_CS_HIGH();
}
uint8_t sFLASH_NOR_ReadLowSR(void)
{
  uint8_t data = 0;
  sFLASH_CS_LOW();
  sFLASH_SendByte(sFLASH_CMD_NOR_RDSR);
  data = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
  sFLASH_CS_HIGH();
  return data;
}
uint8_t sFLASH_NOR_ReadMidSR(void)
{
  uint8_t data = 0;
  sFLASH_CS_LOW();
  sFLASH_SendByte(sFLASH_CMD_NOR_MISR);
  data = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
  sFLASH_CS_HIGH();
  return data;
}
uint8_t sFLASH_NOR_ReadHiSR(void)
{
  uint8_t data = 0;
  sFLASH_CS_LOW();
  sFLASH_SendByte(sFLASH_CMD_NOR_HISR);
  data = sFLASH_SendByte(sFLASH_DUMMY_BYTE);
  sFLASH_CS_HIGH();
  return data;
}
void sFLASH_NOR_WriteLowSR(uint8_t uldata)
{

  sFLASH_WriteEnable();
  sFLASH_CS_LOW();
  sFLASH_SendByte(0x01);
  sFLASH_SendByte(uldata);
  sFLASH_CS_HIGH();
  sFLASH_NOR_WaitForWriteEnd();
}
void sFLASH_NOR_WriteMidSR(uint8_t uldata)
{

  sFLASH_WriteEnable();
  sFLASH_CS_LOW();
  sFLASH_SendByte(sFLASH_CMD_NOR_MISR);
  sFLASH_SendByte(uldata);
  sFLASH_CS_HIGH();
  sFLASH_NOR_WaitForWriteEnd();
}
void sFLASH_NOR_WriteHiSR(uint8_t uldata)
{
  sFLASH_WriteEnable();
  sFLASH_CS_LOW();
  sFLASH_SendByte(sFLASH_CMD_NOR_HISR);
  sFLASH_SendByte(uldata);
  sFLASH_CS_HIGH();
  sFLASH_NOR_WaitForWriteEnd();
}
void sFLASH_NOR_WriteAllSR(uint32_t uldata)
{
  sFLASH_WriteEnable();
  sFLASH_CS_LOW();
  sFLASH_SendByte(sFLASH_CMD_NOR_HISR);
  sFLASH_SendByte((uldata & 0xFF0000) >> 16);
  sFLASH_CS_HIGH();
  sFLASH_WriteEnable();
  sFLASH_CS_LOW();
  sFLASH_SendByte(sFLASH_CMD_NOR_MISR);
  sFLASH_SendByte((uldata & 0xFF00) >> 8);
  sFLASH_CS_HIGH();
  sFLASH_WriteEnable();
  sFLASH_CS_LOW();
  sFLASH_SendByte(sFLASH_CMD_NOR_RDSR);
  sFLASH_SendByte((uldata & 0xFF));
  sFLASH_CS_HIGH();
  sFLASH_WriteEnable();
  sFLASH_CS_LOW();
  sFLASH_SendByte(sFLASH_CMD_NOR_RDSR);
  sFLASH_SendByte((uldata & 0xFF));
  sFLASH_SendByte((uldata & 0xFF00) >> 8);
  sFLASH_CS_HIGH();
  sFLASH_WriteDisable();
}
