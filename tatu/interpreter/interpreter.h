#ifndef interpreter_h
#define interpreter_h

//#define AVR_GCC

// Uncomment the follow line to show debug
#define DEBUG

//#define ENABLE_SOFTWARE_SERIAL

#ifdef DEBUG
#include "debug.h"
#endif
#include <stdint.h>


#ifdef AVR_GCC
#include "Arduino.h"
#include <avr/pgmspace.h>
#endif

#ifdef ESP_GCC
#include "Arduino.h"
#endif

// Change debug port to Software Serial Object if you want to
/*
#ifdef AVR_GCC
    #define DEBUG_PORT	ATMSerial
#endif
// Using ESP 
#ifdef ESP_GCC
    #define DEBUG_PORT	ESPSerial
#endif

#ifdef PROGMEM
    void SerialPrint_PROGMEM(PGM_P str);
#endif
#ifdef ENABLE_SOFTWARE_SERIAL
    #include <SoftwareSerial.h>
    // Debug Software Serial
    SoftwareSerial static DEBUG_PORT(12, 13);//Extra2 == 12 Extra3 == 13    
#endif*/

// Copy property to begin of the payload array
#define VAR_COPY(START, LEN, VECTOR) for(i = START; i < LEN; i++){ VECTOR[i-START] = VECTOR[i]; if(i+1 == LEN) VECTOR[i-START+1] = '\0'; else if(VECTOR[i] == ' ') VECTOR[i-START] = 0; }

/* Utilities */
uint32_t hash_djb(char *string);
uint8_t atoi_T(char *p);
// TATU Protocol avaliable type variables
enum Type {
    TATU_TYPE_ALIAS,
    TATU_TYPE_PIN,
    TATU_TYPE_SYSTEM,
    TATU_TYPE_ANALOG
};

// TATU Protocol available codes
enum Code {
    TATU_CODE_DOD,
    TATU_CODE_STR,
    TATU_CODE_INT,
    TATU_CODE_BOOL,
    TATU_CODE_FLOW
};

//the command amount
#define dataNumber TATU_CODE_BOOL

// Char that represents the TATU Protocol commands
#define CODE_DOD    'D'
#define CODE_STR    'S'
#define CODE_INT    'I'
#define CODE_BOOL   'B'
#define CODE_FLOW   'F'

static uint8_t dataTypes[] = {
    CODE_DOD,
    CODE_STR,
    CODE_INT,
    CODE_BOOL,
    CODE_FLOW
};

// TATU Protocol available commands
enum Commands {
    TATU_POST,
    TATU_SET,
    TATU_GET,
    TATU_FLOW,
    TATU_EDIT
};

//the command amount
#define featNumber TATU_EDIT

// Char that represents the TATU Protocol properties
#define COMMAND_POST 'P'
#define COMMAND_SET  'S'
#define COMMAND_GET  'G'
#define COMMAND_EDIT 'E'
#define COMMAND_FLOW 'F'

static uint8_t features[] = {
    COMMAND_POST,
    COMMAND_SET,
    COMMAND_GET,
    COMMAND_FLOW,
    COMMAND_EDIT
};


class interpreter{
private:

public:
    /**
     * and now
     * what?
    */
    typedef union {
        struct {
            uint8_t ERROR 	: 1;/**< Indicate if there was a error on parse*/
            uint8_t STATE 	: 1;/**< Detailed description after the member */
            uint8_t VAR 	: 6;/**< Detailed description after the member */
            uint8_t CODE 	   ;/**< Detailed description after the member */
            uint8_t TYPE  	   ;/**< Detailed description after the member */
            //uint8_t PIN      ;
        } OBJ;
        uint16_t STRUCTURE;
        void* value;
    } Command;

    Command cmd; ///< Structure that represents the message semantics
    uint32_t str_hash; ///< Hash that identifies the sensor/actuator requested by the client
    interpreter(){
        cmd.OBJ.ERROR = true;
        // Enable Software Serial Debug port if it's not already started 
        #ifdef ENABLE_SOFTWARE_SERIAL
        DEBUG_PORT.begin(DEBUG_PORT_SPEED);
        #endif
    }

    /*
        @return boolean value that represents the validity of the message
    */
    /** 
      * \brief Parses the message
      * \param message That is the message payload
      * \param length represents the message length
      * \return boolean value that represents the validity of the message
    */
    bool parse(char *message, unsigned int length);
    //bool code_evaluation(char, unsigned int*);

    /** 
      * \brief Determine the kind of the request
      * \return boolean value that represents the validity of the message
    */
    bool req(char* ,unsigned int* );
    
    /** 
      * \brief Determine the type of the request value(int, bool, etc...) 
      * \return boolean value that represents the validity of the message
    */
    bool type(char* ,unsigned int* );
    
    /** 
      * \brief Extract the sensor/actuator ID
      * \return boolean value that represents the validity of the message
    */
    bool id(char* ,unsigned int* );
    
    /** 
      * \brief Extract the value of the request(if there is one)
      * \return boolean value that represents the validity of the message
    */
    bool value(char* ,unsigned int* );
};

#endif