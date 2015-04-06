#ifndef TATUInterpreter_h
#define TATUInterpreter_h

#include <stdint.h>


#define TATU_SET    0
#define TATU_GET    1
#define TATU_POST   2
#define TATU_EDIT   3

#define TATU_CODE_ALL   0
#define TATU_CODE_INFO  1
#define TATU_CODE_PROP  2
#define TATU_CODE_STATE 3

#define CODE_ALL   'A'
#define CODE_INFO  'I'
#define CODE_PROP  'P'
#define CODE_STATE 'S'

#define COMMAND_SET  'S'
#define COMMAND_GET  'G'
#define COMMAND_POST 'P'
#define COMMAND_EDIT 'E'

class TATUInterpreter{
private:
    typedef union {
        struct {
            uint8_t STATE : 1;
            uint8_t CODE  : 2;
            uint8_t TYPE  : 2;
            uint8_t PIN   : 3;
        } OBJ;
        uint8_t STRUCTURE;
    } Command;
    bool ERROR;
public:
    Command cmd;
    TATUInterpreter(){ ERROR = true; }
    void Interpreter(unsigned char *, unsigned int);
    bool getERROR();
};

#endif
