#include <avr/io.h>
#include "spi.h"

#define AT_EXECUTE	0	// > 
#define AT_SET      1	// > =
#define AT_TEST     2	// > =?
#define AT_INQUIRE  3	// > ?

#define CRLF1 '\r'
#define CRLF2 '\n'
#define SEND_CRLF() { SPI_WRITE(CRLF1); SPI_WRITE(CRLF2); }

// Read a char from the SPI Communication
char SPI_READ(){
	USIDR = 0;
	USISR = (1 << USIOIF);
	
	for(;(USISR & (1 << USIOIF)) == 0;)
		USICR = (1 << USIWMO) | \
				(1 << USICS1) | \
				(1 << USICLK) | \
				(1 << USITC);

	return USIDR;
}

// Write a char on the SPI Communication
void SPI_WRITE(char send){
	USIDR = send;
	USISR = (1 << USIOIF);
	
	for(;(USISR & (1 << USIOIF)) == 0;)
		USICR = (1 << USIWMO) | \
				(1 << USICS1) | \
				(1 << USICLK) | \
				(1 << USITC);
}

void SPI_WRITE_AT_NS(char *at_command, char *information, uint8_t type){
	int i;

	for(i = 0; at_command[i] != '\0'; i++)
		SPI_WRITE(at_command[i]);

	if(type != AT_EXECUTE){
		if(type == AT_TEST || type == AT_SET)
			SPI_WRITE('=');
		if(type == AT_TEST || type == AT_INQUIRE)
			SPI_WRITE('?');

		for(i = 0; information[i] != '\0'; i++)
			SPI_WRITE(information[i]);
	}
}

void SPI_WRITE_AT_WS(char *at_command, char *information, uint8_t size, uint8_t type){
	int i;

	for(i = 0; at_command[i] != '\0'; i++)
		SPI_WRITE(at_command[i]);

	if(type != AT_EXECUTE){
		if(type == AT_TEST || type == AT_SET)
			SPI_WRITE('=');
		if(type == AT_TEST || type == AT_INQUIRE)
			SPI_WRITE('?');

		for(i = 0; i < size; i++)
			SPI_WRITE(information[i]);
	}
}
