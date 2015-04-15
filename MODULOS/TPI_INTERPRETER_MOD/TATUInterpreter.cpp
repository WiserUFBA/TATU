#include "TATUInterpreter.h"
#include <stdint.h>

#define HASH_DJB(START, LEN, INPUT, OUTPUT) for(i = START; i < LEN; i++){ OUTPUT = ((OUTPUT << 5) + OUTPUT) + INPUT[i]; }

bool TATUInterpreter::parse(unsigned char *string, uint8_t length){
    uint8_t i;
    str_hash = 5381;

    switch(string[0]){
        case COMMAND_SET:
            cmd.OBJ.TYPE = TATU_SET;

            if(string[4] == CODE_INFO){
                cmd.OBJ.CODE = TATU_CODE_INFO;
                HASH_DJB(9, length, string, str_hash);
            }
            else if(string[4] == CODE_STATE){
                cmd.OBJ.CODE = TATU_CODE_STATE;
                cmd.OBJ.PIN = ((uint8_t) string[10]) - 49;
                cmd.OBJ.STATE = ((uint8_t) string[12]) - 49;
            }
            else break;

            ERROR = false;
            return true;
        case COMMAND_GET:
            cmd.OBJ.TYPE = TATU_GET;
            
            if(string[4] == CODE_ALL){
                cmd.OBJ.CODE = TATU_CODE_ALL;
            }
            else if(string[4] == CODE_INFO){
                cmd.OBJ.CODE = TATU_CODE_INFO;
                HASH_DJB(9, length, string, str_hash);
            }
            else if(string[4] == CODE_STATE){
                cmd.OBJ.CODE = TATU_CODE_STATE;
                cmd.OBJ.PIN = ((uint8_t) string[9]) - 49;
            }
            else break;

            ERROR = false;
            return true;
        case COMMAND_EDIT:
            cmd.OBJ.TYPE = TATU_EDIT;
            if(string[5] == CODE_INFO){
                cmd.OBJ.CODE = TATU_CODE_INFO;
                HASH_DJB(10, length, string, str_hash);
            }
            else if(string[5] == CODE_STATE){
                cmd.OBJ.CODE = TATU_CODE_STATE;
                cmd.OBJ.PIN = ((uint8_t) string[11]) - 49;
                cmd.OBJ.STATE = ((uint8_t) string[13]) - 49;
            }
            else break;

            ERROR = false;
            return true;
        case COMMAND_POST:
            cmd.OBJ.TYPE = TATU_POST;
            ERROR = false;
            return true;
    }

    ERROR = true;
    return false;
}

bool TATUInterpreter::getERROR(){
    return ERROR;
}
