#include <avr/io.h>
#include <inttypes.h>
#include "eeprom.h"

char EEPROM_READ(uint16_t end){
    /* espera alguma outra escrita em andamento*/
    for(;EECR & (1<<EEPE););
    /* atribui endereço*/
    EEAR = end;
    /* Começa leitura*/
    EECR |= (1<<EERE);
    /* Retorna dado */
    return EEDR;
}

void EEPROM_READ_NS(uint16_t end, char* p){
    end = EEPROM_READ(end);
    for(;EEPROM_READ(end) != '\0';){
        &p = EEPROM_READ(end);
        p++;
        end++;
    }
    &p = '\0';
    
}

void EEPROM_READ_WS(uint16_t end, char* p, uint8_t tamanho){
    char i;
    end = EEPROM_READ(end);
    for(i = 0; i < tamanho; p++){
        &p = EEPROM_READ(end);
        end++;
    }
}


void EEPROM_WRITE(uint16_t end, char data){
    /* espera alguma outra escrita em andamento*/
    for(;EECR & (1<<EEPE););
	/*Modo de programação (APAGAR E ESCREVER)*/
    EECR = (0<<EEPM1)|(0<<EEPM0)
	/*Atribui endereço/data*/
    EEAR = end;
    EEDR = data;
    /* Escrita a começar*/
    EECR |= (1<<EEMPE);
    /* Começa a escrita*/
    EECR |= (1<<EEPE);
}

void EEPROM_WRITE_NS(uint16_t end, char* vetor){
    end = EEPROM_READ(end);
    for(;&vetor != '\0';){
        EEPROM_WRITE(end, &vetor);
        end++;
        vetor++;
    }
    EEPROM_WRITE(end, (unsigned char) '\0');
}

void EEPROM_WRITE_WS(uint16_t end, char* vetor, uint8_t tamanho){
    uint8_t i;
    end = EEPROM_READ(end);
    for(i = 0; i < tamanho; i++){
        EEPROM_WRITE(end, &vetor);
        end++;
        vetor++;
    }
}
