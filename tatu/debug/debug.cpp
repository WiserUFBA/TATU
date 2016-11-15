#include "debug.h"

const char START_PARSE[]        PROGMEM = "[DEBUG] Starting Parse";
const char FOUND_GET[]          PROGMEM = "[DEBUG] Found GET";
const char FOUND_SET[]          PROGMEM = "[DEBUG] Found SET";
const char FOUND_FLOW[]         PROGMEM = "[DEBUG] Found FLOW";
const char FOUND_FLOW_PUB[]     PROGMEM = "[DEBUG] Found FLOW PUBLISH";
const char FOUND_NUM[]          PROGMEM = "[DEBUG] Is Pin";
const char FOUND_DOD[]          PROGMEM = "[DEBUG] Found DOD";
const char FOUND_EDIT[]         PROGMEM = "[DEBUG] Found EDIT";
const char FOUND_POST[]         PROGMEM = "[DEBUG] Found POST";
const char FOUND_INFO[]         PROGMEM = "[DEBUG] Found INFO";
const char FOUND_VALUE[]        PROGMEM = "[DEBUG] Found VALUE";
const char FOUND_STATE[]        PROGMEM = "[DEBUG] Found STATE";
const char FOUND_SYSTEM[]       PROGMEM = "[DEBUG] Is System";
const char FOUND_ANALOG[]       PROGMEM = "[DEBUG] And is analog";
const char PARAM_VAR[]          PROGMEM = "[DEBUG] Variable received: ";
const char STRING_VAR[]         PROGMEM = "[DEBUG] Parameters String: ";
const char CODE_ERROR[]         PROGMEM = "[DEBUG] Code ERROR";
const char PARAM_VALUE[]        PROGMEM = "[DEBUG] Value received: ";
const char START_CODING[]       PROGMEM = "[DEBUG] Starting coding";
const char HASH_GENERATED[]     PROGMEM = "[DEBUG] Hash generated: ";
const char COMMAND_RECEIVED[]   PROGMEM = "[DEBUG] Command received: ";
const char REQUISITION_ERROR[]  PROGMEM = "[DEBUG] Requisition ERROR";
const char ALL_ERROR[]          PROGMEM = "[DEBUG] The ALL function isn't working yet";

Debug debug;

void standardStream::print(char str){
    cout << str;
}
void standardStream::println(char str){
    cout << str << endl;
}

void standardStream::print(const char* str){
    cout << str;
}
void standardStream::println(const char* str){
    cout << str << endl;
}

void standardStream::print(int str){
    cout << str;
}
void standardStream::println(int str){
    cout << str << endl;
}

/*
    Here we define the debug/print interface using polymorphism
*/

/*static void debug (char* msg){
    cout << msg;    
}
/*static void debug (const char* msg){
    cout << msg;    
}
/*static void debugln (char* msg){
    debug(msg);     
    cout << endl;
}
static void debugln (const char* msg){
    debug(msg);
    cout << endl;
}*/

/*static void debug(char* msg){
//#ifdef DEBUG
    Serial.print(msg);
//#endif
}
static void debug(const char* msg){
//#ifdef DEBUG
    Serial.print(msg);
//#endif
}*/
/*static void debug(const char str[] PROGMEM){
#ifdef DEBUG
    SerialPrint_PROGMEM(str);
    Serial.println();
#endif
}
/*
    The "break-line" prints
*/
/*static void debugln(char* msg){
//#ifdef DEBUG
    Serial.println(msg);
//#endif
}
static void debugln(const char* msg){
//#ifdef DEBUG
    Serial.println(msg);
//#endif
}*/
/*static void debugln(const char str[] PROGMEM){
#ifdef DEBUG
    SerialPrint_PROGMEM(str);
    Serial.println();
#endif
}*/