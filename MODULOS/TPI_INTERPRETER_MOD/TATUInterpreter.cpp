#include "TATUInterpreter.h"

void TATUInterpreter::parse(unsigned char *string, unsigned int length){
    switch(string[0]){
        case COMMAND_SET:
            cmd.OBJ.TYPE = TATU_SET;

            if(string[4] == CODE_INFO){
                cmd.OBJ.CODE = TATU_CODE_INFO;
                // Not implemented yet
            }
            else if(string[4] == CODE_STATE){
                cmd.OBJ.CODE = TATU_CODE_STATE;
                cmd.OBJ.PIN = ((unsigned int) string[10]) - 49;
                cmd.OBJ.STATE = ((unsigned int) string[12]) - 49;
            }
            else break;

            ERROR = false;
            return;
        case cmd_GET:
            cmd.OBJ.TYPE = TATU_GET;
            
            if(string[4] == CODE_ALL){
                cmd.OBJ.CODE = TATU_CODE_ALL;
            }
            else if(string[4] == CODE_INFO){
                cmd.OBJ.CODE = TATU_CODE_INFO;
                // Not implemented yet
            }
            else if(string[4] == CODE_STATE){
                cmd.OBJ.CODE = TATU_CODE_STATE;
                cmd.OBJ.PIN = ((unsigned int) string[9]) - 49;
            }
            else break;

            ERROR = false;
            return;
        case cmd_EDIT:
            cmd.OBJ.TYPE = TATU_EDIT;
            if(string[5] == CODE_INFO){
                cmd.OBJ.CODE = TATU_CODE_INFO;
                // Not implemented yet
            }
            else if(string[5] == CODE_STATE){
                cmd.OBJ.CODE = TATU_CODE_STATE;
                cmd.OBJ.PIN = ((unsigned int) string[11]) - 49;
                cmd.OBJ.STATE = ((unsigned int) string[13]) - 49;
            }
            else break;

            ERROR = false;
            return;
        case cmd_POST:
            cmd.OBJ.TYPE = TATU_POST;
            ERROR = false;
            return;
    }

    ERROR = true;
}

bool TATUInterpreter::getERROR(){
    return ERROR;
}
