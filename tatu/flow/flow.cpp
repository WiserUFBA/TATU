#include "flow.h"

flow::flow(tatu* aux_device, char* aux_response){
	#ifdef FLOW_DEBUG
		DEBUG_PORT.begin(115200);
	#endif
	device = aux_device;
	vector_response = aux_response;
	flow_buffer.end = flow_buffer.vector;
}

void flow::loop() {
	FlowList unit = activity;
	#ifdef FLOW_DEBUG
		int counting = 0;
	#endif
	while (unit) {
		#ifdef COUNTING
			//counts the units being used
			debug("counting: ");
			debugln(counting);
		#endif
		flowIteration(unit);
		unit = unit->next;
	}
}
void flow::flowIteration(FlowList unit){
	
	// If is not used it's not executed
	if (!unit->used) return;

	//"Collect" timeout
	if ((millis() / unit->collect_freq) >= unit->lastTimeCollect) {
		#ifdef FLOW_DEBUG
			debugln("Trying to request...");
			//debugln(counting);
		#endif
	  	unit->lastTimeCollect++;
	  	requisition(vector_iterator(unit), unit->att,unit->type);
	  	#ifdef FLOW_DEBUG
			debugln("Requisition done!");
			//debugln(counting);
		#endif
	}
	//"Shipping" timeout
	if ((millis() / unit->publish_freq) > unit->lastTimePub) {
	  unit->lastTimePub++;
	  //flow_publish(unit);
	  flow_pub(unit);
	  unit->iterator = 0;
	}
}
//increment a iterator and return the previous
void* flow::vector_iterator(FlowList unit) {	

	return ((unit->vector) + (unit->t_size * unit->iterator++));
}

// Reset a iterator
void* flow::iterator_reset(FlowList unit){
	unit->iterator = 0;
}


#ifdef FLOW_DEBUG
void debug_array(int* arr, int length){
	int i;
	debug("Array: [");
	for (i = 0; i < length - 1; ++i){
		debug(arr[i]);
		debug(",");
	}
	debug(arr[i]);
	debugln("]");

}
void debug_array(char arr[][10], int length){
	int i;
	debug("Array: [");
	for (i = 0; i < length - 1; ++i){
		debug(arr[i]);
		debug(",");
	}
	debug(arr[i]);
	debugln("]");

}
#endif

void flow::flow_pub(FlowList unit){
	#ifdef FLOW_DEBUG
		debugln("Publishing...");
		//debug_array((int*)unit->vector,unit->size);

		/*int* arr; 
		(int*)unit->vector;
		for (int i = 0; i < count; ++i)
		{
		}*/
	#endif
	// Polymorphism to handle types
	if(unit->type == INT_T)
		buildResponse((int*)unit->vector,unit->size);
	if(unit->type  == STR_T)
		buildResponse((char(*)[10])unit->vector,unit->size);

	pubResponse(unit);
}

//Who collects the samples(void*)
void flow::requisition(void* response, uint32_t hash,uint8_t code) {
	device->get_function(hash, response, code);
	#ifdef FLOW_DEBUG
		//DEBUG_PORT.begin(115200);
		//debugln(*(int*)response);
	#endif
}


//responseBuilder INTEGER
void flow::buildResponse(int* arr,int length) {
	
	#ifdef FLOW_DEBUG
		debug_array(arr,length);
		debugln("INTEGER!!");
	#endif
	uint8_t aux;//char response Iterator
	uint8_t i;//int* arr Iterator

	// Writes the response on the flow buffer 
	char* response = vector_response;
	response[0] = '\0';

	//<dataManipulation>
	// Array values construction
	for (i = 0; i < length; i++){
		//<block> insert comma
		aux = strlen(response);
		response[aux] = ',';
		response[++aux] = '\0';
		//</block>

		// Insert integer value
		itoa(arr[i], &response[aux], 10);
	}

	//<closeData> Encapsula a resposta nos colchetes
	response[0] = '[';
	aux = strlen(response);
	response[aux] = ']';
	response[++aux] = '\0';
	#ifdef FLOW_DEBUG
		debugln("RESPONSE:");
		debugln(response);
	#endif
	//</closeData>
	//</dataManipulation>
}
//responseBuilder String
void flow::buildResponse(char arr[][10],int length) {
	
	int aux;//char response Iterator
	int i;//int* arr Iterator

	#ifdef FLOW_DEBUG
		debugln("STRING!!");
		debug_array(arr,length);
	#endif

	// Writes the response on the flow buffer 
	char* response = vector_response;
	response[0] = '\0';

	//<dataManipulation>
	// Array values construction
	for (i = 0; i < length; i++){
		//<block> insert comma
		aux = strlen(response);
		response[aux] = ',';
		//OPEN QUOTES
		response[++aux] = '\"';
		response[++aux] = '\0';
		//</block>
		#ifdef FLOW_DEBUG
			//debug_array(arr,length);
			debug("Copiando: ");
			debugln(arr[i]);
		#endif
		// Insert String value
		strcpy(&response[aux],arr[i]);

		//CLOSE QUOTES
		aux = strlen(response);
		response[aux] = '\"';
		response[++aux] = '\0';

	}

	//<closeData> Encapsula a resposta nos colchetes
	response[0] = '[';
	aux = strlen(response);
	response[aux] = ']';
	response[++aux] = '\0';
	#ifdef FLOW_DEBUG
		debugln("RESPONSE:");
		debugln(response);
	#endif
	//</closeData>
	//</dataManipulation>
}
// response "Shipping"
void flow::pubResponse(FlowList unit){
	/*
		req = "SET INFO flow"
		the objective is to call the device->callback function internally
	*/
	byte req[30];
	#ifdef DEBUG_PORT
		DEBUG_PORT.debug("Publish requisition: ");
		DEBUG_PORT.debugln((const char*)unit->message);
	#endif
	strcpy_P((char*)req, (const char*)unit->message);
	device->callback("", req, strlen((char*)req) );	

}


//May be build in future
//void flow::push_response(char* response,){}

// verify if there is a instantiated flow of this sensor
bool flow::isInstantiated(FlowList unit,uint32_t hash){
	while (unit->att != hash && unit->next) {
		unit = unit->next;
	}
	if (unit->att == hash)
		return true;
	return false;
}

//
void flow::flowbuilder(char* json, uint32_t hash, uint8_t code) {


	const int BUFFER_SIZE = JSON_OBJECT_SIZE(3);//needed to determine jsonbuffer size(abstarct it)
	StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);

	#ifdef FLOW_DEBUG
	DEBUG_PORT.begin(115200);
		debug("JSON: ");
		debugln(json);
		debug("collect :"); debugln((int)root["collect"]);
		debug("publish :"); debugln((int)root["publish"]);
		debug("turn :"); debugln((int)root["turn"]);
	#endif
	void* vector;
	FlowList unit = activity;
	//

	//if turn is '1' stop the attribute flow
	if (root["turn"] == 1) {
		while (unit->att != hash) {
		  	unit = unit->next;
		  	// statement
		}
		unit->used = false;
		return;
	}

	//debugln("Checando");
	//Seek for unused flow unit
	while (unit->used) {
		unit = unit->next;
	}
	/*if(!isInstantiated(unit,hash)){
		flow_construct(hash, root["collect"], (void*)get_flow,
	             root["publish"], TATU_GET, code, vector, H_flow, unit);
	}*/

	//set the type
	//uint8_t type = sizeof(int);

	//construct the flow unit
	//debugln("Construindo!");
	//vector = flow_buffer.end;
	flow_construct(hash, root["collect"], get_flow,
	             root["publish"], TATU_GET, code, vector, H_flow, unit);


	//Allocate space on buffer according to the number of samples
	//int *p = new (buffer_alloc) int[unit->size];
	//unit->vector = p;
	//buffer_alloc(unit);
}
void flow::flow_construct(uint32_t hash, int collect_freq, const char* message, int publish_freq, uint8_t code, uint8_t type, void* vector, uint32_t flow, FlowList unit) {
	//This function constructs the flow unit
	#ifdef FLOW_DEBUG
	DEBUG_PORT.begin(115200);
		debugln("Entrou!");
	#endif
	unit->iterator = 0;
	unit->collect_freq = collect_freq;
	unit->publish_freq = publish_freq;
	unit->lastTimeCollect = millis() / collect_freq;
	unit->lastTimePub = millis() / publish_freq;
	unit->att = hash;
	//dynamic array still not implemented
	//unit->vector = vector;
	unit->type = type;
	
	if (type == TATU_CODE_VALUE){
		unit->t_size = sizeof(int);
	}
	if (type == TATU_CODE_INFO){
		unit->t_size = 10;//sizeof(char(*)[10]);
	}
	
	unit->flow = flow;

	//unit->message = message;
	//<workAround>
	int i;
	for (i = 0; i < 2 && (sensors[i].hash != hash); i++);
	//unit->message = message + sensors[i].sensorName;
	/*
		the message
		GET FLOW + temperatureSensor
	*/
	#ifdef DEBUG
		debug("Nome do sensor: ");
		debugln(sensors[i].sensorName);
	#endif	
	strcpy(unit->message,message);
	strcpy(&(unit->message)[strlen(unit->message)],sensors[i].sensorName);
	//</workAround>

	unit->used = true;

	uint8_t size = unit->publish_freq / unit->collect_freq;
	unit->size = size;
	//unit->used = true;
	#ifdef FLOW_DEBUG
		debugln("Construiu!");
	#endif
}

void flow::buffer_alloc(FlowList unit) {
	//Allocate space on buffer according to the number of samples
	unit->vector = flow_buffer.end;
	flow_buffer.end = ((unit->vector) + (unit->t_size * unit->size - 1));

	#ifdef FLOW_DEBUG
		debugln("Alocou!");
	#endif

}

//find the next string in a array composed by strings
int flow::nextStr(char* str){
	int i = 0;
	while(str[i++])
	return i;
}
void* flow::vector_acess(FlowList unit, int i) {

	if (unit->type == STR_T){
		int j,k;
		char* str = (char*)unit->vector;
		for (j = 0; j < i; ++j){
			//nextStr(str,k);
		}
	}

	return ((unit->vector) + (unit->t_size * i));
	
}