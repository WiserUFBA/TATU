#include "TATUInterpreter.h"

TATUInterpreter::Interpreter(unsigned char *string, unsigned int length){
    int i;

    switch(string[0]){
        case COMMAND_SET:
            command.OBJ.TYPE = TATU_SET;

            if(string[4] == CODE_INFO){
                command.OBJ.CODE = TATU_CODE_INFO;
                // Not implemented yet
            }
            else if(string[4] == CODE_STATE){
                command.OBJ.CODE = TATU_CODE_STATE;
                command.OBJ.PIN = ((unsigned int) string[10]) - 49;
                command.OBJ.STATE = ((unsigned int) string[12]) - 49;
            }
            else break;

            ERROR = false;
            return;
        case COMMAND_GET:
            command.OBJ.TYPE = TATU_GET;
            
            if(string[4] == CODE_ALL){
                command.OBJ.CODE = TATU_CODE_ALL;
            }
            else if(string[4] == CODE_INFO){
                command.OBJ.CODE = TATU_CODE_INFO;
                // Not implemented yet
            }
            else if(string[4] == CODE_STATE){
                command.OBJ.CODE = TATU_CODE_STATE;
                command.OBJ.PIN = ((unsigned int) string[9]) - 49;
            }
            else break;

            ERROR = false;
            return;
        case COMMAND_EDIT:
            command.OBJ.TYPE = TATU_EDIT;
            if(string[5] == CODE_INFO){
                command.OBJ.CODE = TATU_CODE_INFO;
                // Not implemented yet
            }
            else if(string[5] == CODE_STATE){
                command.OBJ.CODE = TATU_CODE_STATE;
                command.OBJ.PIN = ((unsigned int) string[11]) - 49;
                command.OBJ.STATE = ((unsigned int) string[13]) - 49;
            }
            else break;

            ERROR = false;
            return;
        case COMMAND_POST:
            command.OBJ.TYPE = TATU_POST;
            ERROR = false;
            return;
    }

    ERROR = true;
}
