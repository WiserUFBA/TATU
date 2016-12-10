#ifndef debug_h
#define debug_h

#ifndef SENSOR_H
#include "sensor.h"
#endif

#define DEBUG
#define AVR_GCC
//#define virtualDev

#ifdef virtualDev
#include <iostream>
#endif
#include <stdint.h>
using namespace std;
// Change debug port to Software Serial Object if you want to
#ifdef AVR_GCC
	#include "Arduino.h"
    #define DEBUG_PORT	ATMSerial
#endif
// Using ESP
//#define ESP_GCC 
#ifdef ESP_GCC
	#include "Arduino.h"
    #define DEBUG_PORT	ESPSerial
#endif

#ifdef PROGMEM
    //void SerialPrint_PROGMEM(PGM_P str);
#endif
#ifndef PROGMEM
	#define PROGMEM 
#endif

#ifdef ENABLE_SOFTWARE_SERIAL
    #include <SoftwareSerial.h>
    // Debug Software Serial
    SoftwareSerial static DEBUG_PORT(12, 13);//Extra2 == 12 Extra3 == 13    
#endif

/* DEBUG! */
//#ifdef DEBUG
const char GENERATE_BODY[]          PROGMEM = "[DEBUG] Generating Body";
const char CALLBACK_INFO[]          PROGMEM = "[DEBUG] INFO function is being executed";
const char CALLBACK_VALUE[]         PROGMEM = "[DEBUG] VALUE function is being executed";
const char CALLBACK_STATE[]         PROGMEM = "[DEBUG] STATE function is being executed";
const char GET_PIN[]                PROGMEM = "[DEBUG] The value of the pin is: ";
const char SET_PIN[]                PROGMEM = "[DEBUG] The value of the pin has been set";
const char THE_INFO_RESPONSE[]      PROGMEM = "[DEBUG] The response for the GET INFO requisition is: ";
const char THE_VALUE_RESPONSE[]     PROGMEM = "[DEBUG] The response for the GET VALUE requisition is: ";
const char THE_STATE_RESPONSE[]     PROGMEM = "[DEBUG] The response for the GET STATE requisition is: ";
const char BODY_GENERATED[]         PROGMEM = "[DEBUG] The body generation is done";
const char IS_A_POST[]              PROGMEM = "[DEBUG] It's a post so it doesn't have to publish anything";
const char PUBLISHING[]             PROGMEM = "[DEBUG] Publishing...";
const char PUBLISHED[]              PROGMEM = "[DEBUG] The message has been published";
const char NOT_A_GET[]              PROGMEM = "[DEBUG] It isn't a GET requisition";
const char SYSTEM[]                 PROGMEM = "[DEBUG] The system function isn't working yet";
const char THE_RESPONSE[]           PROGMEM = "[DEBUG] The value of the response is: ";
const char INITIATING[]             PROGMEM = "[DEBUG] Initianting the class...";
const char FINISHED_INIT[]          PROGMEM = "[DEBUG] Finished init!";
const char STARTING_GENERATE[]      PROGMEM = "[DEBUG] Starting generate HEADER...";
const char ENDING_GENERATE[]        PROGMEM = "[DEBUG] Finished generate HEADER!";
const char HEADER_STR[]             PROGMEM = "[DEBUG] HEADER Value : ";
const char CLASS_CONSTRUCTED[]      PROGMEM = "[DEBUG] Class constructed with success!";
const char EXEC_ERROR[]             PROGMEM = "[DEBUG] Execution Error!";
const char EXEC_ERROR_TYPE_VAR[]    PROGMEM = "[DEBUG] Unknown variable type!";
const char PARAM_ERROR[]            PROGMEM = "[DEBUG] Param Error!";
const char RESPONSE_TYPE_STR[]      PROGMEM = "[DEBUG] The response type is STR";
const char RESPONSE_TYPE_INT[]      PROGMEM = "[DEBUG] The response type is INT";
const char RESPONSE_TYPE_BOOL[]     PROGMEM = "[DEBUG] The response type is BOOL";
const char DOD_RETURN[]             PROGMEM = "[DEBUG] Returning the following DOD Object";
//#endif


extern class Debug debug;

#ifdef virtualDev
class standardStream{
public:
	//standardStream();
	template <typename T> 
  	void println(T x){ cout << x << endl;};

};
#endif

#ifdef virtualDev
class Debug: public standardStream{
public:
	//Debug();

	//singleton
	static Debug& getInstance()	{
	}

};
#endif
#ifdef AVR_GCC
class Debug{
public:
	//Debug();
	//singleton
	Debug(Stream* s){
		m_dev = s;
	}
	template <typename T> 
	void println(T x){ 
		#ifdef DEBUG
		m_dev->println(x);
		#endif
	};
	static Debug& getInstance()	{
	}

protected:
	Stream* m_dev;
	 
	virtual size_t write(uint8_t c) { return (m_dev->write(c)); }
	virtual int available(void) { return (m_dev->available()); }
	virtual int peek(void) { return (m_dev->peek()); }
	virtual int read(void) { return (m_dev->read()); }
	virtual void flush(void) { m_dev->flush(); }
};

#endif


class tatuTester{
private:
    LDR ldr;
    TemperatureSensor temp;
public:

    //TemperatureSensor temp;

    tatuTester() : ldr(1),temp(2){

        //ldr = new LDR(1);
    }

    bool strSolver(uint32_t hash,void* response);
    bool intSolver(uint32_t hash,void* response);

    bool get(uint32_t hash, void* response, uint8_t code);
    bool set(uint32_t hash, uint8_t code, void* request);
    void publish(char* payload, char* topic);
};


#endif

