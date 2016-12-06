#include "debug.h"
#include "Interpreter.h"
#include "Device.h"


char message[] = "GET BOOL lamp";
char device_name[] = "device01";

bool getSwitch(){

}

bool get(uint32_t hash, void* response, uint8_t code){
    debug.println("No gET");


    switch (code){
    case TYPE_CODE_STR:
        debug.println("STRING");
        strcpy((char*)response,"1");
        break;
    case TYPE_CODE_INT:
        *(int*)response = 1;
        break;
    case TYPE_CODE_BOOL:
        *(bool*)response = true;
        break;
    default:
        return false;
    }

    return true;
}
bool set(uint32_t hash, uint8_t code, void* request){
    debug.println("No SET");
    return true;
}
void publish(char* payload, char* topic){

}
Interpreter interpreter;
Device device(device_name,&interpreter,get,set,publish);

void testDevice(){
    device.generateBody(message,strlen(message));
    debug.println("Terminando");
    debug.println(device.output_message);
    debug.println((int)interpreter.cmd.CODE);
    //debug();
}
void testSensor(){
    //interpreter.parse(message,strlen(message));
    debug.println("LDR:");
    debug.println(LDR::name);
    debug.println("TemperatureSensor:");
    debug.println(TemperatureSensor::name);
}


void setup(){

    Serial.begin(9600);
    delay(1000);
    testDevice();
}
void loop()
{
	
}


