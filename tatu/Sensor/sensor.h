#ifndef SENSOR_H
#define SENSOR_H



class Sensor{
public:
    //const char* name;
    Sensor();
    virtual void handle(char* x) = 0;///< Dispatcher to handle String request
    virtual void handle(int* x) = 0;///< Dispatcher to handle Integer request
    virtual void handle(bool* x) = 0;///< Dispatcher to handle Boolean request
};

class LDR : public Sensor{
public:
    static const char* name;
    LDR () {}
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
};

class TemperatureSensor : public Sensor{
public:
    static const char* name;
    TemperatureSensor() {}
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
    //TemperatureSensor() {name = "TemperatureSensor";}
};

#endif // SENSOR_H
