#ifndef SENSOR_H
#define SENSOR_H

/*#ifndef debug_h
#include "debug.h"
#endif*/


#define virtualDev

#ifdef virtualDev
#define reading 1
#endif
#ifdef AVR_GCC
#define reading (digital == true) ? digitalRead(pin) : analogRead(pin)
#endif

class Sensor{
public:
    //const char* name;
    int pin;
    bool digital;
    Sensor();
    virtual void handler(char* x) = 0;///< Dispatcher to handle String request
    virtual void handler(int* x) = 0;///< Dispatcher to handle Integer request
    virtual void handler(bool* x) = 0;///< Dispatcher to handle Boolean request
};

/*class LDR : public Sensor{
public:
    static const char* name;
    LDR (int pin) {this->pin = pin;}
    void handle(char* x){
        return;
    }
    void handle(int* x){
        *x = 1;
        return;
    }
    void handle(bool* x){
        return;
    }
    //LDR () {name = "LDR";}
};*/
class LDR : public Sensor{
public:

    static const char* name;
    LDR (int pin) {this->pin = pin;}

    void handler(char *x){
        *x = 0;
    }
    void handler(int *x){
        *x = (4*5*reading);
    }
    void handler(bool *x){
        *x = true;
    }
};



class TemperatureSensor : public Sensor{
public:
    static const char* name;
    TemperatureSensor (int pin) {this->pin = pin;}
    void handler(char* x){
        return;
    }
    void handler(int* x){
        *x = 1;
        return;
    }
    void handler(bool* x){
        return;
    }
    //TemperatureSensor() {name = "TemperatureSensor";}
};

#endif // SENSOR_H
