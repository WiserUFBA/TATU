#ifndef tatu_h
#define tatu_h

#include <stdint.h>
#include <interpreter.h>
#include <debug.h>
#include "Arduino.h"
#ifdef AVR_GCC
#include <avr/wdt.h>
#endif

typedef uint8_t byte;

#ifndef MAX_SIZE_RESPONSE
#define MAX_SIZE_RESPONSE   768
#endif

#ifndef MAX_SIZE_OUTPUT
#define MAX_SIZE_OUTPUT     1024
#endif

// System definitions
#ifdef AVR_GCC
#define PROGMEM                     __ATTR_PROGMEM__
#endif
#ifdef ESP_GCC
#define PROGMEM
#endif
#define OUT_STR             &output_message[aux]
#define MAX_SIZE_NAME       20
#define MQTTPORT_STANDARD   1883

// Operations used for message building
#define COMMA       output_message[aux++]=','
#define COLON       output_message[aux++]=':'
#define QUOTE       output_message[aux++]='\"'
#define BRACE_LEFT  output_message[aux++]='{'
#define BRACE_RIGHT output_message[aux++]='}'
#define CLOSE_MSG   output_message[aux]=0

// Copy a string
#define STRCPY(INPUT, OUTPUT) do{ for(i = 0;INPUT[i] != 0; ++i) OUTPUT[i] = INPUT[i]; OUTPUT[i] = 0; }while(0)
// Copy string starting from a index
#define STRCPY_I(INPUT, OUTPUT, INDEX) do{ for(i = INDEX;INPUT[i - INDEX] != 0; ++i) OUTPUT[i] = INPUT[i - INDEX]; OUTPUT[i] = 0; }while(0)

// Verify if a string is empty
#define ISEMPTY(VAR) (VAR[0] == 0)

// Creates wrapper for the class' callback function
                                        // Declares bridge function
#define MQTT_BRIDGE(BRIDGE)             void BRIDGE(char *, char *)
                                        // Declaração da ponte// Função callback chamada pelo cliente mqtt  
#define MQTT_CALLBACK(OBJ, NAME) void NAME(char *topic, byte *payload, unsigned int length)\ 
                                        {OBJ.callback(topic, payload, length);}// Essa é a atribuição da função acionada dentro do objeto TATUDevice
                                        // Atribuição da ponte
#define MQTT_PUBLISH(BRIDGE, OBJ)       void BRIDGE(char *topic, char *out)\ 
                                        { OBJ.publish(topic,out); } // Essa é a função publish do cliente que será chama pelo objeto

// Macros para conversão de tipos
#define STOS(STRING1,STRING2)   strcpy((char*)STRING1,STRING2)
#define ITOS(INTEGER,STRING)    (itoa(INTEGER,(char*)STRING,10))
#define ITOI(INTEGER1,INTEGER2) *(int*)INTEGER2 = INTEGER1
#define BTOS(BOOL1,STRING)      if (BOOL1) strcpy((char*)STRING,"ON"); else strcpy((char*)STRING,"OFF");
#define BTOB(BOOL1,BOOL2)       *(bool*)BOOL2 = BOOL1

int foo();
void cpyStrConstant(char* destination, const char* source );
void error_message(int aux);
void sucess_message(int aux);

class tatu{
public:
    // Atributos públicos
    // Atributos do sistema

    char        name[MAX_SIZE_NAME];
    int         len_name;
    char        aux_topic_name[MAX_SIZE_NAME + 15];
    bool        dod_used;
    bool (*get_function)(uint32_t hash, void* response, uint8_t code);
    bool (*set_function)(uint32_t hash, uint8_t type, void* request);
    bool (*flow_function)(uint32_t hash, uint8_t code, void* response);
    void (*pub)(char *, char *);
    // Atributos variaveis
    interpreter *requisition;

    /* TEORICO */
    /* uint8_t reset_counter;
    uint8_t start_counter; */

    // Mensagem de saida
    char output_message[MAX_SIZE_OUTPUT];
    int last_char;

    /* Callback's do Sistema */
    // Callback MQTT
    void callback(char *, byte *, unsigned int);
    
    tatu( const char *name_d, interpreter *req, bool (*GET_FUNCTION)(uint32_t hash, void* response, uint8_t code), 
                        bool (*SET_FUNCTION)(uint32_t hash, uint8_t code, void* request), void (*PUBLISH)(char *, char *));

    void init(  const char *name_d, interpreter *req); 

    //
	void tatu_get(void* buffer);
	void tatu_set(void* request);
	void tatu_flow(void* request);

    void sucess_message(int aux);
    void error_message(int aux);

    void generateHeader();
    void generateBody(char *payload, uint8_t length);
    void loop();
};

#endif