#include <iostream>
#include <string.h>
#include "debug.h"
#include "interpreter.h"

using namespace std;

interpreter device;

char message[] = "SET BOOL lamp false";
//char message[] = "SET INT temp 22";

int main(int argc, char const *argv[]){
	debug.println(features[0]);

	device.parse(message,strlen(message));
	//debug();
	return 0;
}
