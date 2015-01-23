#include <inttypes.h>

#define MAC         0x00
#define IP          0x06
#define CHANNEL     0X0A
#define SENHA       0X0B
#define BSSID       

// Write a byte into and specified eeprom address
void EEPROM_WRITE(uint16_t, char);
// Write a string with a non specified size into an eeprom array
void EEPROM_WRITE_NS(uint16_t, char *);
// Write a string with a specified size into an eeprom array 
void EEPROM_WRITE_WS(uint16_t, char *, uint8_t tamanho);

// Read a byte from an specified eeprom address
char EEPROM_READ(uint16_t);
// Read a string with a non specified size from an eeprom array
char EEPROM_READ_NS(uint16_t, char *);
// Read a string with a specified size from an eeprom array
char EEPROM_READ_WS(uint16_t, char *, uint8_t tamanho);

