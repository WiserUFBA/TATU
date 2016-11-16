#include "interpreter.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int nextFunc(char* string,int next){
    int i; 

    for (i = next; string[i] != '\0' && string[i] != ' ' ; i++);

    return i+1;
}

#ifdef AVR_GCC
#include <avr/pgmspace.h>
#endif

/* Generate the djb hash */
uint32_t hash_djb(char *string){
    uint32_t hash = 5381;
    int i;
    for(i = 0; string[i] != 0 && string[i] != ' '; i++){ hash = ((hash << 5) + hash) + string[i]; }
    return hash;
}

/* Array to String **Optimized */
uint8_t atoi_T(char *p){
    int k = 0;
    while(*p){k = (k << 3) + (k << 1) + (*p) - '0'; p++; }
    return k;
}

/* Parse TATU and return if this fail or not */
bool interpreter::parse(char *string, unsigned int length){
    
    
    /*
        Messages format: <req> <type> <id> <value>(when set or flow)

        <example>
            GET INT temperatureSensor
            FLOW STR currentSensor01 {"collect":500,"publish":2500}
        </example>

        @param i Iterator
        @param next The iterator that indicates the next wprd in the message 
        @param cmd Struct that represents the message meaning 
    */
    unsigned int i, next = 0;
    /*
        Theses are some static changes, for compatibility
    */
    cmd.STRUCTURE = 0;
    cmd.OBJ.VAR = TATU_KIND_ALIAS;
    cmd.OBJ.TYPE = 1;

    // <req> evaluation
    if (!req(string,&next)) return false;
    if (!type(string,&next)) return false;
    
    // Extract sensor actuactor ID
    id(string,&next);

    // If is a "GET" there is no extra value
    if(cmd.OBJ.CODE != COMMAND_GET)
        if (!value(string,&next)) return false;


    return true;
}

//verify requisiton type
bool interpreter::req(char* string,unsigned int* next){
    int i;

    /* 
        check if the requisition's first letter belongs to 
        the list of characters of the protocol requisitions
    */
    for ( i = 0; i < featNumber && (features[i] != string[0]) ; i++);

    if (i == featNumber){
        return false;
    }
    //cout << "at least here" << endl;
    debug.println("at least here");
    //debug.println((char)features[i]);

    // Insert the request type in the command structure
    cmd.OBJ.CODE = i;
    debug.println(cmd.OBJ.CODE);
    // Atributtes the size of this part to find the next requisition's string
    *next = nextFunc(string,*next);
    debug.println(&string[*next]);

    return true;
}
bool interpreter::type(char* string,unsigned int* next){
    int i;

    /* 
        check if the requisition's first letter belongs to 
        the list of characters of the protocol data types
    */
    for ( i = 0; i < dataNumber && (dataTypes[i] != string[*next]) ; i++);

    if (i == dataNumber){
        return false;
    }
    debug.println("at least here");
    //debug.println((char)dataTypes[i]);

    cmd.OBJ.TYPE = i;
    debug.println(cmd.OBJ.TYPE);

    // Atributtes the size of this part to find the next requisition's string
    *next = nextFunc(string,*next);
    debug.println(&string[*next]);
    return true;
}
bool interpreter::id(char* string,unsigned int* next){
    str_hash = hash_djb(&string[*next]);
    *next = nextFunc(string,*next);
    debug.println((int)str_hash);
} 

bool interpreter::value(char* string,unsigned int* next){
    debug.println("Foi");
    
    cmd.value = valueParser[cmd.OBJ.TYPE].valueFunc(&string[*next]);
    debug.println(cmd.value);
}
//bool interpreter::

/*bool interpreter::code_evaluation(char code,unsigned int *j){
    switch(code){
        case CODE_DOD:
            #ifdef DEBUG
            PRINT_DEBUG(FOUND_DOD);
            DEBUG_NL();
            //PRINT_DEBUG(ALL_ERROR);
            //DEBUG_NL();
            #endif
            cmd.OBJ.CODE = TATU_CODE_DOD;
            break;
        case CODE_STR:
            #ifdef DEBUG
            PRINT_DEBUG(FOUND_INFO);
            DEBUG_NL();
            #endif
            cmd.OBJ.CODE = TATU_CODE_STR;
            break; 
        case CODE_BOOL:
            #ifdef DEBUG
            PRINT_DEBUG(FOUND_STATE);
            DEBUG_NL();
            #endif
            cmd.OBJ.CODE = TATU_CODE_BOOL;
            *j += 1;
            break; 
        case CODE_INT:
            #ifdef DEBUG
            PRINT_DEBUG(FOUND_VALUE);
            DEBUG_NL();
            #endif
            cmd.OBJ.CODE = TATU_CODE_INT;
            *j += 1;
           break;
        case CODE_FLOW:
            #ifdef DEBUG
            PRINT_DEBUG(FOUND_FLOW_PUB);
            DEBUG_NL();
            #endif
            cmd.OBJ.CODE = TATU_CODE_FLOW;
            break; 
        default:
            #ifdef DEBUG
            PRINT_DEBUG(CODE_ERROR);
            DEBUG_NL();
            #endif
            /* If the desired command is not found return error */
            /*return false;
    }
    return true;
}*/