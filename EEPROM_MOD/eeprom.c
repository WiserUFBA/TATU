#include <avr/io.h>

void escrever(unsigned int end, unsigned char data){
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


unsigned char ler(unsigned int end){
  /* espera alguma outra escrita em andamento*/
  for(;EECR & (1<<EEPE););
  /* atribui endereço*/
  EEAR = end;
  /* Começa leitura*/
  EECR |= (1<<EERE);
  /* Retorna dado */
  return EEDR;
}
