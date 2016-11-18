#include "debug.h"
#include "interpreter.h"


interpreter device;
char message[] = "SET STR lamp false";

void setup(){
	Serial.begin(9600);
	debug.println((char)features[0]);
	device.parse(message,strlen(message));
	//debug()
}
void loop()
{
	
}


