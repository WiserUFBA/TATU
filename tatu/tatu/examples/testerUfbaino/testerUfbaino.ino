#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <tatu.h>
#include <debug.h>
#include <interpreter.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <flow.h>
//#includ

// Pins used
#define LAMP    13

/*
  Sensor ID's defines
*/
//  <sensorsID>
#define ID_current  0
#define ID_temp     1
#define ID_humid    2
#define ID_lumin    3
#define ID_lamp     4
//  </sensorsID>

//Hash that represents the attributes
#define H_temperatureSensor 0x5821FBAD
#define H_luminositySensor  0xF8D7A29C
#define H_currentSensor01   0x1A5B1C23
#define H_humiditySensor    0x83D62D4C

//
sensorMap sensors[4];

// Update these with values suitable for your network.
const char*   device_name     = "ufbaino04";
const char*   mqtt_user       = "wiser";
const char*   mqtt_pass       = "wiser2014";
const int     mqttport        = 1883;

// Constants to connection with the broker
const char*   ssid            = "wiser";          //"PiratasDoValeDoCanela"
const char*   password        = "wiser2014";      //"naovaqueebarril"
const char*   mqtt_server     = "192.168.0.112";  //"192.168.0.103"
char          subsc_topic[20] = "dev/";

//Hash that represents the attributes
#define H_lamp      2090464143

//flow
#define H_flow              0x7C96D85D

//workaround
byte ip[5] = "11";
//
char vector_response[1024];


bool get(uint32_t hash, void* response, uint8_t code) {
  uint8_t id;

  if (code == TATU_CODE_FLOW){
    STOS(response, vector_response);
    return true;
  }

  switch (hash) {
    case H_currentSensor01:
      id = ID_current;
      break;
    case H_temperatureSensor:
      // The dht_temperatures_sensor supports INFO and VALUE requests.
      id = ID_temp;
      break;
    case H_humiditySensor:
      // The dht_humidity_sensor supports INFO and VALUE requests.
      id = ID_humid;
      break;
    case H_luminositySensor:

      // The lumisity_sensor supports INFO and VALUE,requests.
      id = ID_lumin;
      break;
    case H_flow:
      STOS(response, vector_response);
      return true;
      break;
    default:
      return false;
  }

  //<bus>
  char bus_buffer[20];
  request(id, 's');
  //read_serial((int*)response,id);
  double bus_res;
  read_serial(&bus_res, id);
  //Handling types
  //int
  if (code == TATU_CODE_INT) {
    //*(int*)response = atoi(bus_buffer);
    *(int*)response = bus_res;
    ////debug("Response: ");
    ////debugln(*(int*)response);
  }
  //char*
  if (code == TATU_CODE_STR) {
    //STOS(response,bus_buffer);
    ITOS(bus_res, response);
    if(id == ID_current){
      dtostrf(bus_res,2,2,(char*)response);   
    }
    ////debug("Response: ");
    ////debugln((char*)response);
  }
  //</bus>

  return true;
}


bool set(uint32_t hash, uint8_t code, void* response) {
  switch (hash) {
    case H_lamp:
      //bool_actuator(LAMP,lamp,response,code);
      break;
    default:
      return false;
  }

  return true;
}

// System variables
WiFiClient    espClient;
interpreter interpreter;
MQTT_BRIDGE(bridge);
PubSubClient  client(espClient);
tatu device(device_name, &interpreter, get, set, bridge);
MQTT_CALLBACK(device, mqtt_callback);
MQTT_PUBLISH(bridge, client);

flow fluxo(&device, vector_response);
FlowUnit unit1, unit2, unit3, unit4;

// //debug Software Serial
//SoftwareSerial ESPSerial(12, 13);                 //Extra2 == 12 Extra3 == 13

bool flow_function(uint32_t hash, uint8_t code, void* response) {
  //debugln("Flow!!");
  fluxo.flowbuilder((char*)response, hash, code);
  return true;
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  //debugln();
  //debug("Connecting to ");
  //debugln(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //debug(".");
  }

  //debugln("");
  //debugln("WiFi connected");
  //debugln("IP address: ");
  //debugln(WiFi.localIP());

}


void request(int id, uint8_t type) {

  char req[8];
  sprintf(req, "%c:%d", type, id);

  /*//debug("Message arrived [");
    //debug(topic);
    //debug("] ");*/

  //Envia mensagem que chegou
  /*for (int i = 0; i < strlen(req); i++) {
    ESPSerial.write(payload[i]);
    Serial.write(payload[i]);
    }*/
  //debug("Mensagem enviada: ");
  //debugln(req);
  Serial.println(req);
}

//Predicado que verifica se o id de retorno está correto
bool pred_id(char* topic, int id) {
  int count = 0;
  if ((atoi(topic) != id)) {
    //debugln("Mensagem Errada!");
    while (!Serial.available() && count < 10) {
      count++;
      delay(50);
    }
    return true;
  }

  return false;
}

void read_serial(double* response, int id) {
  int i;
  int count = 0;
  char msg[255];
  char topic[20];

  do {
    count++;
    if (count > 6) break;

    delay(50);
    //debugln("ATMEGA Response");
    //debug("Topic = ");
    for (i = 0; Serial.available(); i++ ) {
      topic[i] = Serial.read();
      if (topic[i] == '>') break;
      //debug(topic[i]);
    }
    //debugln();

    topic[i] = 0;

    //debug("Message = ");
    for (i = 0; Serial.available(); i++) {
      msg[i] = Serial.read();
      //debug(msg[i]);
      if (msg[i] == '\n') break;
    }

    msg[i] = 0;

    //fail safety
  } while (pred_id(topic, id));

  //int
  *response = atof(msg);
  //debug("Response: ");
  //debugln(*response);
  //strcpy(response,msg);

}

void reconnect() {

  // Loop until we're reconnected
  while (!client.connected()) {
    //debug("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(device_name, mqtt_user, mqtt_pass)) {
      //debugln("connected");
      // Once connected, publish an announcement...
      client.publish("CONNECTED", device_name);
      // ... and resubscribe
      client.subscribe("dev/");
      client.subscribe(subsc_topic);
    }
    else {
      //debug("failed, rc = ");
      //debug(client.state());
      //debugln(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }

}

const char set_flow[] = "FLOW INFO temperatureSensor {\"collect\":400,\"publish\":2000}";
const char set_f_luminosity[] = "FLOW INFO luminositySensor {\"collect\":400,\"publish\":2000}";
int buffer_int_flow[100];
//char buffer_char_flow[100][10];
char buffer_char_flow[100][10];
char buffer_char_flow2[100][10];
void setup() {

  foo();
  //Serial.begin(115200);
  ////debug_ṔORT.begin(115200);
  //ESPSerial.write("Estou no setup!");

  //flow settings
  device.flow_function = &flow_function;
  unit1.vector = buffer_char_flow;
  unit2.vector = buffer_char_flow2;
  unit1.used = false; unit2.used = false;
  fluxo.activity = &unit1;
  unit1.next = &unit2;
  //
  //map settings
  strcpy(sensors[0].sensorName,"temperatureSensor");
  strcpy(sensors[1].sensorName,"currentSensor01");
  strcpy(sensors[2].sensorName,"luminositySensor");
  strcpy(sensors[3].sensorName,"humiditySensor");
  //sensors[0].sensorName = "temperatureSensor";
  //sensors[1].sensorName = "currentSensor01";

  sensors[0].hash = H_temperatureSensor;
  sensors[1].hash = H_currentSensor01;
  sensors[2].hash = H_luminositySensor;
  sensors[3].hash = H_humiditySensor;
  fluxo.sensors = sensors;
  //

  //CONNECTIONS
  setup_wifi();

  strcpy(&subsc_topic[4], device_name);

  client.setServer(mqtt_server, 1883);
  client.setCallback(mqtt_callback);

  client.subscribe("dev/");
  client.subscribe(subsc_topic);

  client.publish("dev/CONNECTIONS", device_name);
  /*byte req[80];

    strcpy((char*)req, set_flow);
    device.mqtt_callback("", req, strlen((char*)req) );
    strcpy((char*)req, set_f_luminosity);
    device.mqtt_callback("", req, strlen((char*)req) );*/
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  //ESPSerial.write("Cheguei no loop!");
  client.loop();
  fluxo.loop();

}
