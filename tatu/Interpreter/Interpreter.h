#ifndef interpreter_h
#define interpreter_h


// Uncomment the follow line to show debug
//#define DEBUG
//#define AVR_GCC

//#define ENABLE_SOFTWARE_SERIAL

//#ifdef DEBUG
#include "debug.h"
//#endif
#include <stdint.h>

//#define AVR_GCC
#ifdef AVR_GCC
#include "Arduino.h"
#include <avr/pgmspace.h>
#endif

#ifdef ESP_GCC
#include "Arduino.h"
#endif

// Copy property to begin of the payload array
#define VAR_COPY(START, LEN, VECTOR) for(i = START; i < LEN; i++){ VECTOR[i-START] = VECTOR[i]; if(i+1 == LEN) VECTOR[i-START+1] = '\0'; else if(VECTOR[i] == ' ') VECTOR[i-START] = 0; }

/* Utilities */
uint32_t hash_djb(char *string);
uint8_t atoi_T(char *p);
// TATU Protocol avaliable type variables
enum Kind {
    TATU_KIND_ALIAS,
    TATU_KIND_PIN,
    TATU_KIND_SYSTEM,
    TATU_KIND_ANALOG
};

//

// TATU Protocol available type codes
enum typeCode {
    TYPE_CODE_DOD,
    TYPE_CODE_STR,
    TYPE_CODE_INT,
    TYPE_CODE_BOOL,
    TYPE_CODE_FLOW
};

//the command amount
#define dataNumber TYPE_CODE_FLOW

// Char that represents the TATU Protocol commands
#define TYPE_DOD    'D'
#define TYPE_STR    'S'
#define TYPE_INT    'I'
#define TYPE_BOOL   'B'
#define TYPE_FLOW   'F'

///Functions to handle type
static uint64_t getString(char *message){
    debug.println("String");
    return (uint64_t)message;
}
static uint64_t getFlow(char *message){
    return getString(message);
}
static uint64_t getInteger(char *message){
    debug.println("Integer");
    return (uint64_t)atoi_T(message);
}
static uint64_t getBool(char *message){
    return message[0] == 't'? true : false;
}

typedef uint64_t (*getV)(char* V);

/*
 * This struct is used to properly handle the value extraction
*/
typedef struct {
    char type;///< The first character of the type
    typeCode code;///< The code that represents the type
    getV valueFunc;///< The function tha handle the extraction
} ValueParser;

const ValueParser valueParser[] = {
        {TYPE_DOD,TYPE_CODE_DOD,NULL},
        {TYPE_STR,TYPE_CODE_STR,getString},
        {TYPE_INT,TYPE_CODE_INT,getInteger},//
        {TYPE_BOOL,TYPE_CODE_BOOL,getBool},
        {TYPE_FLOW,TYPE_CODE_FLOW,getFlow}//the flow will handle the json
};

static uint8_t dataTypes[] = {
    TYPE_DOD,
    TYPE_STR,
    TYPE_INT,
    TYPE_BOOL,
    TYPE_FLOW
};


// TATU Protocol available commands
enum Commands {
    COMMAND_CODE_POST,
    COMMAND_CODE_SET,
    COMMAND_CODE_GET,
    COMMAND_CODE_FLOW,
    COMMAND_CODE_EDIT
};

//the command amount
#define featNumber COMMAND_CODE_EDIT

// Char that represents the TATU Protocol properties
#define COMMAND_POST 'Q'
#define COMMAND_SET  'S'
#define COMMAND_GET  'G'
#define COMMAND_FLOW 'F'
#define COMMAND_EDIT 'E'

static uint8_t features[] = {
    COMMAND_POST,
    COMMAND_SET,
    COMMAND_GET,
    COMMAND_FLOW,
    COMMAND_EDIT
};


class Interpreter{
private:

    /**
     * Structure that represents the message semantics
    */
    typedef struct {
        //struct {
        uint8_t ERROR;/**< Indicate if there was a error on parse*/
        uint8_t VAR;/**< Detailed description after the member */
        uint8_t CODE;/**< Determine the request kind */
        uint8_t TYPE;/**< Determine the request type(int,bool, etc...) */
        //} OBJ;
        uint16_t STRUCTURE;
        uint64_t object;
        uint64_t value;/**< Determine the message value*/
    } Command;
public:


    Command cmd; ///< Structure that represents the message semantics
    uint32_t str_hash; ///< Hash that identifies thse sensor/actuator requested by the client
    Interpreter(){
        cmd.ERROR = true;
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

    //void setCode(uint8_t );
};

#endif
