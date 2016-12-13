#include "debug.h"

#ifdef virtualDev
Debug debug;
#endif

#ifdef AVR_GCC
Debug debug(&Serial);
#endif

/*
 * DEFINES TEMPORARIOS
 */
enum typeCode {
    TYPE_CODE_DOD,
    TYPE_CODE_STR,
    TYPE_CODE_INT,
    TYPE_CODE_BOOL,
    TYPE_CODE_FLOW
};

/*
 * tatuTester
 */
#define H_ldr 193498023
#define temp_hash 2
bool tatuTester::strSolver(uint32_t hash,void* response){
    switch (hash){
    case H_ldr:
        ldr.handler((int*)response);
        break;
    case temp_hash:
        temp.handler((char*)response);
        break;
    default:
        return false;
    }
}
bool tatuTester::intSolver(uint32_t hash, void *response){
    switch (hash){
    case H_ldr:
        debug.println("Aki!!");
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
    debug.println("No GET");
    switch (code){
    case TYPE_CODE_STR:
        strSolver(hash,response);
        break;
    case TYPE_CODE_INT:
        intSolver(hash,response);
        break;
    case TYPE_CODE_BOOL:
        *(bool*)response = true;
        break;
    default:
        return false;
    }

    return true;
}
bool tatuTester::set(uint32_t hash, uint8_t code, void* request){
    debug.println("No SET");
    return true;
}
void tatuTester::publish(char* topic,char* payload){
    debug.print(topic);
    debug.print(": ");
    debug.println(payload);
    return;
}

void tatuTester::inputTests(){
    string line;
    while(getline(cin,line)){
        debug.println(line);
    }
}


