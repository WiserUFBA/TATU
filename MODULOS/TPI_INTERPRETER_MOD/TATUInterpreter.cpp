#include "TATUInterpreter.h"

bool TATUInterpreter::parse(unsigned char *string, unsigned int length){
    int i;
    switch(string[0]){
        case COMMAND_SET:
            cmd.OBJ.TYPE = TATU_SET;

            if(string[4] == CODE_INFO){
                cmd.OBJ.CODE = TATU_CODE_INFO;
                for (i = 0; i < 6; ++i){   
                        str_val[i] = string[9+i];
                }
                if(i > 6) break;
                str_val[i] = '\0';
            }
            else if(string[4] == CODE_STATE){
                cmd.OBJ.CODE = TATU_CODE_STATE;
                cmd.OBJ.PIN = ((unsigned int) string[10]) - 49;
                cmd.OBJ.STATE = ((unsigned int) string[12]) - 49;
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
                for (i = 0; i < 6; ++i){   
                        str_val[i] = string[9+i];
                }
                if(i > 6) break;
                str_val[i] = '\0';
            }
            else if(string[4] == CODE_STATE){
                cmd.OBJ.CODE = TATU_CODE_STATE;
                cmd.OBJ.PIN = ((unsigned int) string[9]) - 49;
            }
            else break;

            ERROR = false;
            return true;
        case COMMAND_EDIT:
            cmd.OBJ.TYPE = TATU_EDIT;
            if(string[5] == CODE_INFO){
                cmd.OBJ.CODE = TATU_CODE_INFO;
                for (i = 0; i < 6; ++i){   
                        str_val[i] = string[10+i];
                }
                if(i > 6) break;
                str_val[i] = '\0';
            }
            else if(string[5] == CODE_STATE){
                cmd.OBJ.CODE = TATU_CODE_STATE;
                cmd.OBJ.PIN = ((unsigned int) string[11]) - 49;
                cmd.OBJ.STATE = ((unsigned int) string[13]) - 49;
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
