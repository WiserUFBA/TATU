#include <avr/io.h>
#include <avr/pgmspace.h>
// General Includes
#include <inttypes.h>
#include <stdio.h>
// User-specified lib
#include "avr-thread.h"
#include "tatu_buf.h"
#include "spi.h"
#include "eeprom.h"
#include "tps_define.h"

#define ATTINY_84   0
#define ATMEGA_328  1
#define ATMEGA_168  2
#define ATMEL_1281  3

#define CRLF1 '\r'
#define CRLF2 '\n'
#define SEND_CRLF() { SPI_WRITE(CRLF1); SPI_WRITE(CRLF2); }

#define AT_EXECUTE  0   // > 
#define AT_SET      1   // > =
#define AT_TEST     2   // > =?
#define AT_INQUIRE  3   // > ?

// Define which uC we are using
#define MCU_USED // Don't forget to choose one from the list above

#ifdef  MCU_USED == ATTINY_84
// ... Some defines here
#elif   MCU_USED == ATMEGA_328
// ... More defines
#elif   MCU_USED == ATMEGA_168
// ... And more
#elif   MCU_USED == ATMEGA_1281
// ... and this is the last!
#endif

// General ESP8266 config
// #define ESP8266_BAUDRATE 

/*
 * At Commands for the ESP8266
 * 
 * COMMAND      |   PARAMETERS   |  SPECIFICATION
 *
 * AT+RST                         Restart the module
 *
 * AT+CWMODE    mode              Wifi mode
 *
 * AT+CWJAP     ssid,pwd          Join the network
 *
 * AT+CWQAP                       Quit the network
 *
 * AT+CWSAP     ssid,chl=channel, Set the parameters of AP
 *              encryption=enc,pwd
 * AT+CIPSTATUS                   Get the connection status
 *
 * AT+CIPSTART  IP address=addr   Set up TCP or UDP connection 
 *              port,id,type
 * AT+CIPSEND                     Send data
 *
 * AT+CIPCLOSE                    Close TCP or UDP connection
 * 
 * AT+CIFSR                       Get IP address
 *
 * AT+CIPMUX    set               Set mutiple connection
 *
 * AT+CIPSERVER set               Set as server
 *
 * +IPD                           Received data 
*/

char *AT_COMMANDS[] = { "AT+RST\0",         \
                        "AT+CWMODE\0",      \
                        "AT+CWJAP\0",       \
                        "AT+CWQAP\0",       \
                        "AT+CWSAP\0",       \
                        "AT+CIPSTATUS\0",   \
                        "AT+CIPSTART\0"     \
                        "AT+CIPSEND\0",     \
                        "AT+CIPCLOSE\0",    \
                        "AT+CIFSR\0",       \
                        "AT+CWJAP\0",       \
                        "AT+CIPMUX\0",      \
                        "AT+CIPSERVER\0",   \
                    };

// Size 256 bytes
/* Defines the shared buffer memory */
union MEMORY_TYPE_BUFFER{
    // 1 Byte Buffer ~ 8bits
    volatile BUFFER_UINT_SIZE_256 UINT_SIZE_256;
    volatile BUFFER_UINT_SIZE_128 UINT_SIZE_128;
    volatile BUFFER_UINT_SIZE_64  UINT_SIZE_64;
    volatile BUFFER_UINT_SIZE_32  UINT_SIZE_32;
    volatile BUFFER_UINT_SIZE_16  UINT_SIZE_16;
    volatile BUFFER_UINT_SIZE_8   UINT_SIZE_8;
    volatile BUFFER_UINT_SIZE_4   UINT_SIZE_4;
    // 2 Byte Buffer ~ 16bits
    volatile BUFFER_UINT2_SIZE_128 UINT2_SIZE_128;
    volatile BUFFER_UINT2_SIZE_64  UINT2_SIZE_64;
    volatile BUFFER_UINT2_SIZE_32  UINT2_SIZE_32;
    volatile BUFFER_UINT2_SIZE_16  UINT2_SIZE_16;
    volatile BUFFER_UINT2_SIZE_8   UINT2_SIZE_8;
    volatile BUFFER_UINT2_SIZE_4   UINT2_SIZE_4;
    // 4 Byte Buffer ~ 32bits
    volatile BUFFER_UINT4_SIZE_64  UINT4_SIZE_64;
    volatile BUFFER_UINT4_SIZE_32  UINT4_SIZE_32;
    volatile BUFFER_UINT4_SIZE_16  UINT4_SIZE_16;
    volatile BUFFER_UINT4_SIZE_8   UINT4_SIZE_8;
    volatile BUFFER_UINT4_SIZE_4   UINT4_SIZE_4;
} UC_BUFFER;

union MEMORY_TYPE_CONFIG{
    /* ... */
} UC_CONFIG;

// STACK
/* 
 *  --> 128 <--
 * 1--> 16b > NETWORK_COMMUNICATION
 * 2--> 16b > 
 * 3--> 16b
 * 4--> 16b
 * 5--> 16b
 * 6--> 16b
 * 7--> 16b
 * 8--> 16b
 */

 // 512b
 /*
  *
  * BUFFER 256 bytes
  * ------  64 bytes // NON ALOCATED
  * CONFIG  32 bytes
  * SHARED  32 bytes
  * THREAD 128 bytes
  *
  */

// Context for each thread
avr_thread_context NETWORK_COMMUNICATION_CONTEXT;

// Switch count for threads
uint32_t switch_count;

// Stack for each THREAD
uint8_t NETWORK_STACK[16];

void SIG_OUTPUT_COMPARE2(void) __attribute__((naked));

void SIG_OUTPUT_COMPARE2(void){
    sei();
    avr_thread_isr_start();
    switch_count++;
    avr_thread_isr_end();
}

void CONFIG(){
    // Enable SPI Communication
    // SET PA6 - DI ~ MISO
    DDRA &= ~(1 << PA6);
    PORTA |= (1 << PA6);
    // SET PA5 - DO ~ MOSI
    DDRA |= (1 << PA5);
    // SET PA4 - SCK 
    DDRA |= (1 << PA4);

    // Read the MAC ADDRESS
    EEPROM_READ_WS(0, UC_BUFFER.UINT_SIZE_8.BUFFER0, 6);
    // Read the IP
    EEPROM_READ_WS(1, UC_BUFFER.UINT_SIZE_8.BUFFER1, 4);
    //Channel
    EEPROM_READ_WS(2, UC_BUFFER.UINT_SIZE_8.BUFFER2, 1);
    //senha
    EEPROM_READ_NS(3, UC_BUFFER.UINT_SIZE_32.BUFFER2, 21);
    //bssid
    EEPROM_READ_WS(4, UC_BUFFER.UINT_SIZE_8.BUFFER3, 6);
    //ESSID
    EEPROM_READ_WS(5, UC_BUFFER.UINT_SIZE_32.BUFFER3, 32);
    //encryption
    EEPROM_READ_WS(6, UC_BUFFER.UINT_SIZE_8.BUFFER4, 1);
    //NETMASK
    EEPROM_READ_WS(7, UC_BUFFER.UINT_SIZE_8.BUFFER5, 4);
    //gateway
    EEPROM_READ_WS(8, UC_BUFFER.UINT_SIZE_8.BUFFER6, 4);
    // Send MAC
    SPI_WRITE_AT_NS(AT_COMMANDS[6], UC_BUFFER.UINT_SIZE_8.BUFFER1 + BUFFER2 + BUFFER3, AT_SET);

}

void NETWORK_COMMUNICATION(void){

}

int main(){
    CONFIG();

    avr_thread_init();
    avr_thread_start(&NETWORK_COMMUNICATION_CONTEXT, \
                    NETWORK_COMMUNICATION,           \
                    NETWORK_STACK,                   \
                    sizeof(NETWORK_STACK));

    for(;;);
}

