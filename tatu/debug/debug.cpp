#include "debug.h"

#ifdef virtualDev
Debug debug;
#endif

#ifdef AVR_GCC
Debug debug(&Serial);
#endif

/*
 * tatuTester
 */
#define ldr_hash 1
#define temp_hash 2
bool tatuTester::strSolver(uint32_t hash,void* response){
    switch (hash){
    case ldr_hash:
        ldr.handler((int*)response);
        break;
    case temp_hash:
        temp.handler((char*)response);
        break;
    default:
        return false;
    }
}

bool tatuTester::get(uint32_t hash, void* response, uint8_t code){
    debug.println("No gET");


    /*switch (code){
    case TYPE_CODE_STR:
        strSolver(hash,response);
        break;
    case TYPE_CODE_INT:
        ldr.handler((int*)response);
        break;
    case TYPE_CODE_BOOL:
        *(bool*)response = true;
        break;
    default:
        return false;
    }*/

    return true;
}
bool tatuTester::set(uint32_t hash, uint8_t code, void* request){
    debug.println("No SET");
    return true;
}
void tatuTester::publish(char* payload,char* topic){
    return;
}


