#include "gd32f30x.h"
#include "CR95HF.h"
#include "gd32f30x_gpio.h"
#include <stdarg.h>

void RFID_Init(void)
{
    // clock
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_SPI0);
    rcu_periph_clock_enable(RCU_AF);
    // pins
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, RFID_SPI_SCK_PIN | RFID_SPI_MOSI_PIN);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, RFID_SPI_MISO_PIN);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, RFID_CS_PIN);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
    RFID_CS_HIGH();

    spi_parameter_struct spi_init_struct;
    spi_nss_output_enable(RFID_SPI);

    /* SPI0 parameter config */
    spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode = SPI_MASTER;
    spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss = SPI_NSS_SOFT;
    spi_init_struct.prescale = SPI_PSC_256;
    spi_init_struct.endian = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);

    spi_enable(RFID_SPI);
}
void RFID_CR95HF_Reset(void)
{
    RFID_IRQ_LOW();
    delay_1ms(100);
    RFID_IRQ_HIGH();
}

uint8_t RFID_SendByte(uint8_t byte)
{
    /*!< Loop while DR register in not emplty */
    while (spi_i2s_flag_get(RFID_SPI, SPI_FLAG_TBE) == RESET)
        ;

    /*!< Send byte through the SPI1 peripheral */
    spi_i2s_data_transmit(RFID_SPI, byte);

    /*!< Wait to receive a byte */
    while (spi_i2s_flag_get(RFID_SPI, SPI_FLAG_RBNE) == RESET)
        ;

    /*!< Return the byte read from the SPI bus */
    return spi_i2s_data_receive(RFID_SPI);
}

void RFID_CR95HF_Send(uint8_t cmnd, uint8_t len, ...)
{
    va_list args;
    va_start(args, len);
    RFID_CS_LOW();
    RFID_SendByte(0x00);
    RFID_SendByte(cmnd);
    RFID_SendByte(len);
    for (int i = 0; i < len; i++)
    {
        uint8_t val = va_arg(args, int);
        RFID_SendByte(val); // G?i d? li?u
    }
    RFID_CS_HIGH();
    va_end(args);
}

bool RFID_CR95HF_Receive()
{
    //usart_string_transmit(RFID_UART, "Polling");
    RFID_CS_LOW();
    delay_1ms(1);

    RXBuffer[1] = 64;

    while ((RXBuffer[0] != 8) && (RXBuffer[1] != 0))
    {
        RXBuffer[0] = RFID_SendByte(0x03); // Write 3 until
        // Serial.print(RXBuffer[0], HEX);
        // Serial.print(" ");
        RXBuffer[0] = RXBuffer[0] & 0x08; // bit 3 is set
        delay_1ms(10);
        RXBuffer[1]--;
    }

    RFID_CS_HIGH();
    delay_1ms(1);

    if (RXBuffer[1] == 0)
        return 0;
    RFID_CS_LOW();

    RFID_SendByte(0x02);
    RXBuffer[0] = RFID_SendByte(0);
    RXBuffer[1] = RFID_SendByte(0);
    char tmp_string[100];

    sprintf(tmp_string, "Response Code %x Data len %x \n", RXBuffer[0], RXBuffer[1]);
    //sart_string_transmit(RFID_UART, tmp_string);

    if (RXBuffer[1] >= sizeof(RXBuffer) - 2)
        RXBuffer[1] = 0;
    for (int i = 0; i < RXBuffer[1]; i++)
    {
        RXBuffer[i + 2] = RFID_SendByte(0);
    }

    RFID_CS_HIGH();

    delay_1ms(1);
    return 1;
}

/*-------------------------------------------
            COMMAND
---------------------------------------------
*/

bool RFID_Hibernate_Command()
{
    RFID_CR95HF_Send(0x07, 14, 0x08, 0x04, 0x00, 0x04, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    delay_1ms(10);
    RFID_CR95HF_Receive();
}

bool RFID_IDN_Command()
{
    RFID_CR95HF_Reset();
    uint8_t i = 0;
    // step 1 send the command
    RFID_CR95HF_Send(0x01, 0);
    delay_1ms(10);
    RFID_CR95HF_Receive();

    if ((RXBuffer[0] == 0) && (RXBuffer[1] == 15))
    {
        char string_tmp[20];
        sprintf(string_tmp, "DEVICE ID: ");
        //usart_string_transmit(RFID_UART, "DEVICE ID: ");
        for (i = 2; (RXBuffer[i] != '\0') && (i < (RXBuffer[1])); i++)
        {
            //usart_string_transmit(RFID_UART, RXBuffer[i]);
            // Serial.print(char(RXBuffer[i]));
        }
        i++;
        sprintf(string_tmp, " ROM CRC: %x%x\r\n", RXBuffer[i], RXBuffer[i + 1]);
        //usart_string_transmit(RFID_UART, string_tmp);
        return 1;
    }
    else
    {
        char string_tmp[20];
        sprintf(string_tmp, "BAD RESPONSE TO IDN COMMAND!%x%x\r\n", RXBuffer[0], RXBuffer[1]);
        usart_string_transmit(RFID_UART, string_tmp);
        return 0;
    }
}

bool Select_ISO_IEC_14443_A_Protocol()
{
    RFID_CR95HF_Send(ProtocolSelect, 2, 0x02, 0x00);
    RFID_CR95HF_Receive();
    if ((RXBuffer[0] == 0) & (RXBuffer[1] == 0))
    {
        //usart_string_transmit(RFID_UART, "PROTOCOL SET-\r\n");
        return 1;
    }
    else
    {
        //usart_string_transmit(RFID_UART, "BAD RESPONSE TO SET PROTOCOL\r\n");
        return 0;
    }
}

char* RFID_GetTagID(void)
{
    char tmp[100] = {0};
    char str[100] = {0};
    int i = 0;
    memset(str, 0, 100);

    RFID_CR95HF_Send(SendRecv, 2, 0x26, 0x07);
    RFID_CR95HF_Receive();

    sprintf(str, "REQA[0]: %x\r\n", RXBuffer[0]);
    //usart_string_transmit(RFID_UART, str);
    sprintf(str, "REQA[1]: %x\r\n", RXBuffer[1]);
    //usart_string_transmit(RFID_UART, str);

    memset(str, 0, 100);
    //usart_string_transmit(RFID_UART, "ATQA: ");
    for (i = 2; i < (RXBuffer[1] + 2); i++)
    {
        sprintf(str, "%x", RXBuffer[i]);

        //usart_string_transmit(RFID_UART, str);
    }
    //usart_string_transmit(RFID_UART, "\r\n");
    delay_1ms(1);
    RFID_CR95HF_Send(SendRecv, 3, 0x93, 0x20, 0x08);
    RFID_CR95HF_Receive();

    if (RXBuffer[0] == 0x80)
    {
        for (i = 0; i < RXBuffer[1]; i++)
        {
            sprintf(tmp, "%02x", RXBuffer[i + 2]);
            strcat(str, tmp);
        }
    }
    //usart_string_transmit(RFID_UART, "ID: ");
    //usart_string_transmit(RFID_UART, str);
    //usart_string_transmit(RFID_UART, "\r\n");
		
		return str;
}

bool RFID_set_para()
{
    RFID_CR95HF_Send(0x09, 4, 0x3A, 0x00, 0x60, 0x04);
    RFID_CR95HF_Receive();
    if ((RXBuffer[0] == 0) & (RXBuffer[1] == 0))
    {
        //usart_string_transmit(RFID_UART, "SET PARA OK\r\n");
        return 1;
    }
    else
    {
        //usart_string_transmit(RFID_UART, "SET PARA FAIL\r\n");
        return 0;
    }
}

bool RFID_set_gain()
{
    RFID_CR95HF_Send(0x09, 4, 0x68, 0x01, 0x01, 0xD1);
    RFID_CR95HF_Receive();
    if ((RXBuffer[0] == 0) & (RXBuffer[1] == 0))
    {
        //usart_string_transmit(RFID_UART, "SET GAIN OK\r\n");
        return 1;
    }
    else
    {
        //usart_string_transmit(RFID_UART, "SET GAIN FAIL\r\n");
        return 0;
    }
}