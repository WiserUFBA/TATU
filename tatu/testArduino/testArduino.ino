#include "debug.h"
#include "Interpreter.h"
#include "Device.h"


char message[] = "GET INT ldr";
char device_name[] = "device01";

LDR ldr(1);
TemperatureSensor temp(2);

bool getSwitch(){

}

tatuTester tester;

bool get(uint32_t hash, void* response, uint8_t code){
    return tester.get(hash, response, code);
}
bool set(uint32_t hash, uint8_t code, void* request){

    return tester.set(hash, code, request);
}
void publish(char* payload,char* topic){
    tester.publish(payload,topic);
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


