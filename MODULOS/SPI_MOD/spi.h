/* spi.h -- Defines SPI Communication */

// Read a char from the SPI Communication
char SPI_READ();
// Write a char on the SPI Communication
void SPI_WRITE(char);
void SPI_WRITE_AT_NS(char *, char *, uint8_t);
void SPI_WRITE_AT_NS(char *, char *, uint8_t, uint8_t);
