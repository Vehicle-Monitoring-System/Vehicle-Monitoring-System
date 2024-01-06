#include "tcp.h"
#include "atc_e25.h"
#include "gd32f30x.h"
#include "gd32f30x_gpio.h"
#include "gd32f30x_rcu.h"
#include "systick.h"
#include "gd32f30x_exti.h"
#include "string.h"
#include "math.h"
#include "GPS.h"

char old[20];

struct Information
{
    char deviceId[10];
    char driverId[20];
    int year;
    int month;
    int day;
    int hour;
    int minutes;
    int second;
    float latitude;
    float longitude;
    double locationInfo;
    int satelliteNum;
    double speed;
    double distance;
    int status_of_device;
};

void removeAsterisks(char **str)
{
    char *result = NULL;
    char *src = *str;
    int len = strlen(src);
    result = (char *)malloc(len + 1); // Cấp phát bộ nhớ cho chuỗi kết quả

    int j = 0;
    for (int i = 0; i < len; i++)
    {
        if (src[i] != '.')
        {
            result[j] = src[i];
            j++;
        }
    }

    free(*str);    // Giải phóng bộ nhớ của chuỗi ban đầu
    *str = result; // Cập nhật con trỏ để trỏ vào chuỗi kết quả
}

// Function to convert a string to its hexadecimal representation
void string_to_hex(const char *input_string, char *hex_representation)
{
    int i, j = 0;
    size_t len = strlen(input_string);

    for (i = 0; i < len; ++i)
    {
        // Format each character as a two-digit hexadecimal representation
        sprintf(&hex_representation[j], "%02x", input_string[i]);
        j += 2;
    }

    // Null-terminate the resulting hexadecimal string
    hex_representation[j] = '\0';
}

void tcp_init(void)
{
    char rec[1500];
    char *CLOSE = "AT+QICLOSE=0\r\n\0";
    usart_string_transmit(USART2, CLOSE);
    usart_receive_string(USART2, rec, 1500);
    usart_string_transmit(UART3, rec);
    delay_1ms(500);
    char *TCPOPEN = "AT+QIOPEN=1,0,\"TCP\",\"198.37.116.25\",80,0,1\r\n0";
    usart_string_transmit(USART2, TCPOPEN);
    usart_receive_string(USART2, rec, 1500);
    usart_string_transmit(UART3, rec);
    delay_1ms(500);
    char *QSTATE = "AT+QISTATE=1,0\r\n\0";
    usart_string_transmit(USART2, QSTATE);
    usart_receive_string(USART2, rec, 1500);
    usart_string_transmit(UART3, rec);
    delay_1ms(500);
}

void tcp_login(gps_t *gh)
{
    // AT+QISENDEX=0,"24240000140011092310263286418005181236605648542e312e303013dd014dc200d66c2323"
    char login[1400];
    uint8_t hour_temp = gh->hours;
    if ((hour_temp) <= 17)
    {
        gh->hours += 7;
    }
    else
    {
        gh->hours -= 17;
    }
    sprintf(login, "AT+QISENDEX=0,\"242400001400%02d%02d%02d%02d%02d%02d86418005187320005648542e312e303013dd014dc200d66c2323\"\r\n", gh->date, gh->month, gh->year, gh->hours, gh->minutes, gh->seconds);
    // char *login = "AT+QISENDEX=0,\"24240000140012092310255086418005187320005648542e312e303013dd014dc200d66c2323\"";
    usart_string_transmit(USART2, login);
    // char rec[1500];
    // usart_receive_string(USART2, rec, 1500);
    // usart_string_transmit(UART3, rec);
}

void tcp_assemble(gps_t *gh, char *rfid)
{	
		// TIme
    uint8_t hour_temp = gh->hours;
    char rec1[300];

    

		// GPS Speed
    gps_float_t gps_speed = gps_to_speed(gh->speed, gps_speed_kph);
		
		// HTTP request
		char request[1024];
    struct Information sendingToServer;
		// assign
		
		 if (strcmp(rfid, "") != 0 && strcmp(old, "") == 0)
			 {
				 memset(old, '\0', sizeof(old));
				 strcpy(old, rfid);
			 }
			else if (strcmp(rfid, "") != 0 && strcmp(rfid, old) == 0)
				{
					memset(old, '\0', sizeof(old));
            strcpy(old, "");
				}

        // Gán giá trị của old cho trường driverId của sendingToServer
		strcpy(sendingToServer.driverId, old);
		
		
		snprintf(sendingToServer.deviceId, sizeof(sendingToServer.deviceId), "XYZ456");
    snprintf(sendingToServer.driverId, sizeof(sendingToServer.driverId), "%s", old);
    sendingToServer.year = gh->year;
    sendingToServer.month = gh->month;
    sendingToServer.day = gh->date;
    sendingToServer.hour = gh->hours;
    sendingToServer.minutes = gh->minutes;
    sendingToServer.second = gh->seconds;
		sendingToServer.latitude = gh->latitude;
		sendingToServer.longitude = gh->longitude;
    sendingToServer.locationInfo = gh->gp_sats_in_view_desc[0].snr;
    sendingToServer.satelliteNum = gh->sats_in_use;
    sendingToServer.speed = gh->speed;
    sendingToServer.distance = 0;
		// update led
		if ((gh->speed ) >1 && strcmp(old, "") != 0){
		    sendingToServer.status_of_device = 1;
				atc25_led_on(GREEN);
		} else if ((gh->speed ) <=1 && strcmp(old, "") != 0){
				sendingToServer.status_of_device = 2;
				atc25_led_on(GREEN);
				delay_1ms(100);
				atc25_led_off(GREEN);
		}else{
				sendingToServer.status_of_device = 3;
				atc25_led_off(GREEN);
		}
		
		 if (gh->latitude == 0.0) {
			atc25_led_off(BLUE);
			} else{
				atc25_led_on(BLUE);
			}
		
		if ((strcmp(old, "") == 0)){
			atc25_led_off(RED);
		} else{
			atc25_led_on(RED);
		}

		
		char request1[256];
    int length = snprintf(request1, sizeof(request1),
             "{\r\n"
             "\"deviceId\": \"%s\",\r\n"
             "\"driverId\": \"%s\",\r\n"
             "\"time\": \"20%d-%02d-%02dT%02d:%02d:%02dZ\",\r\n"
             "\"latitude\": \"%.6f\",\r\n"
             "\"longitude\": \"%.6f\",\r\n"
             "\"locationInfo\": %.2lf,\r\n"
             "\"satelliteNum\": %d,\r\n"
             "\"speed\": %.2lf,\r\n"
             "\"distance\": %.2lf,\r\n"
             "\"status\": %2d\r\n}",
             sendingToServer.deviceId,
             sendingToServer.driverId,
             sendingToServer.year,
             sendingToServer.month,
             sendingToServer.day,
             sendingToServer.hour,
             sendingToServer.minutes,
             sendingToServer.second,
             sendingToServer.latitude,
             sendingToServer.longitude,
             sendingToServer.locationInfo,
             sendingToServer.satelliteNum,
             sendingToServer.speed,
             sendingToServer.distance,
             sendingToServer.status_of_device);
    
    int length2 = snprintf(request, sizeof(request),
             "POST /api/Gps HTTP/1.1\r\n"
             "Content-Type: application/json\r\n"
             "User-Agent: PostmanRuntime/7.36.0\r\n"
             "Accept: */*\r\n"
             "Postman-Token: 93d42b18-a33c-49e8-939f-d786f0f2803f\r\n"
             "Host: gps-iot.somee.com\r\n"
             "Accept-Encoding: gzip, deflate, br\r\n"
             "Connection: keep-alive\r\n"
             "Content-Length: %d\r\n\r\n%s\r\n",
             length, request1);
		
		delay_1ms(500);
		usart_string_transmit(USART1, request);
		char hex_representation[2 * length2 + 1];
		char at[2 * length2 + 30];
    string_to_hex(request, hex_representation);
		hex_representation[2 * length2] = '\0';
		sprintf(at, "AT+QISENDEX=0,\"%s\"\r\n", hex_representation);
		
		//usart_string_transmit(USART1, at);
		
    char *CLOSE = "AT+QICLOSE=0\r\n\0";
    usart_string_transmit(USART2, CLOSE);
    delay_1ms(500);
    char *TCPOPEN = "AT+QIOPEN=1,0,\"TCP\",\"198.37.116.25\",80,0,1\r\n0";
    usart_string_transmit(USART2, TCPOPEN);
    delay_1ms(500);
    char *QSTATE = "AT+QISTATE=1,0\r\n\0";
    usart_string_transmit(USART2, QSTATE);
    delay_1ms(500);
		
    usart_string_transmit(USART2, at);
		/*
		usart_string_transmit(USART2, at);
    delay_1ms(500);
		*/
		
    // sprintf(at, "AT+QISENDEX=0,\"2424ff003c00110923102632210405721054866347060d000102030400006c00000000000020030f8000011925000000000000000000000000000000000000000000000000000000240048e72323\"\r\n\0");
    //usart_string_transmit(USART2, at);
    // usart_receive_string(USART2, rec1, 300);
    //usart_string_transmit(USART1, at);
		/*
		if (sendingToServer.status_of_device == 1)
			delay_1ms(6000);
		else{
			delay_1ms(60000);
		}
		*/
}



  /* sprintf(at, "AT+QISENDEX=0,\"2424"
                "ff00"
                "3c00"
                "%02d%02d%02d%02d%02d%02d"
                "%08s%0010s"
                "07%02x%02.0f"
                "01020304" // Tổng khoảng cách di chuyển hiện tại đang fix cứng
                "00"
                "00"
                "6c"
                "00"
                "0000"
                "0000"
                "FF20030f8000"                                         // 6 byte này hiện tại fix cứng
                "0119250000000000000000000000000000000000000000000000" // Khôn sử dụng 26 byte
                "0000"
                "0000"
                "2400"
                "48e7"
                "2323"
                "\"\r\n",
            gh->date, gh->month, gh->year, gh->hours, gh->minutes, gh->seconds, la, long1, gh->sats_in_use, gh->speed);*/
		
			
		