#include "Device.h"

#ifdef AVR_GCC
#define //debugString(VAR) const char VAR[] PROGMEM
#endif
#ifdef ESP_GCC
#define //debugString(VAR) const char VAR[] PROGMEM
#endif

// Constantes
const char start_post[] PROGMEM = "{\"CODE\":\"POST\"";
const char null_body[]  PROGMEM = "\"BODY\":null}";
const char true_body[]  PROGMEM = "\"BODY\":true}";
const char false_body[] PROGMEM = "\"BODY\":false}";
const char header_str[] PROGMEM = "\"HEADER\":{";
const char name_str[]   PROGMEM = "\"NAME\":\"";
const char id_str[]     PROGMEM = "\"ID\":";
const char pan_str[]    PROGMEM = "\"PAN\":";
const char ip_str[]     PROGMEM = "\"IP\":\"";
const char body_str[]   PROGMEM = "\"BODY\":{";
const char method_str[] PROGMEM = "\"METHOD\":";
const char method_get[] PROGMEM = "GET";
const char method_set[] PROGMEM = "SET";
const char true_str[]   PROGMEM = "true";
const char false_str[]  PROGMEM = "false";
const char null_str[]   PROGMEM = "null";
const char pin_str[]    PROGMEM = "PIN";
const char dev_str[]    PROGMEM = "dev/";
const char int_str[]    PROGMEM = "/INT";
const char res_str[]    PROGMEM = "/RES";
const char flow_str[]   PROGMEM = "/FLOW";


Device::Device( const char *name_d, Interpreter *req, bool (*GET_FUNCTION)(uint32_t hash, void* response, uint8_t code), 
                        bool (*SET_FUNCTION)(uint32_t hash, uint8_t code, void* request), void (*PUBLISH)(char *, char *)){
    get_function = GET_FUNCTION;
    set_function = SET_FUNCTION;
    pub = PUBLISH;
    init(name_d,req);
}

/* Initialize the class */
void Device::init(  const char *name_d, Interpreter *req){
    
    //cli();//stop interruptions on arduino
    int i;

    // Define basic attributes
    i = strlen(name_d);
    len_name = i + 4;
    STRCPY(name_d, name);

    cpyStrConstant(aux_topic_name,dev_str);
    strcpy(&aux_topic_name[strlen(aux_topic_name)],name_d);
    
    requisition = req;

    // Gera o header padrão e coloca no output_message atualizando a posição final do header
    generateHeader();
    //sei();

    dod_used = false;

    // Enable Software Serial //Debug port if it's not already started 
    #ifdef ENABLE_SOFTWARE_SERIAL
    //DEBUG_PORT.begin(//DEBUG_PORT_SPEED);
    #endif
}

/* Generate the header post */
void Device::generateHeader(){
    /* Auxiliary variable */
    int aux;

    // Primeiro se coloca a seguinte string padrão no vetor
    cpyStrConstant(output_message, start_post);
    
    // Starts JSON
    aux = strlen(start_post);
    COMMA;
    
    // The next lines builds the HEADER
    cpyStrConstant(OUT_STR, header_str); /* Copies the "HEADER" string*/
    aux += 10;
    
    /* Inserts NAME */
    cpyStrConstant(OUT_STR, name_str);
    aux += 8;
    strcpy(OUT_STR, name);
    aux += strlen(name);
    QUOTE; COMMA;

    /* Closes the header */
    BRACE_RIGHT; COMMA;
    CLOSE_MSG;

    /*
        "HEADER":{"NAME":"<name>"}
    */

    last_char = aux;
}

#define APAGA
void Device::generateBody(char *payload, uint8_t length){

	/*
		@response is a pointer to the reponse returned by @get_funtcion()
		@request is a pointer to the value of the request sended to @set_funtcion()
		@str_buffer buffer used to store string responses
		@aux indicates the index of the last message's character edited
	*/
   	void *response;
   	void *request;

    char str_buffer[MAX_SIZE_RESPONSE] = {0};
    char buffer[MAX_SIZE_RESPONSE] = {0};//?

	int aux = last_char;

    #ifndef APAGA
    /*
    	The Interpreter recieves the incoming requisition(@payload) and parses it
    	if it's not a well formed tatu message the body becames BODY:"null"
	*/
    if(!requisition->parse(payload, length)){ cpyStrConstant(OUT_STR, null_body); return; }

	 /* Check the requisiton type*/
    switch(requisition->cmd.OBJ.TYPE){
    	case TATU_GET:
    		tatu_get(buffer);
    		break;
    	case TATU_SET:
    		tatu_set(&payload[strlen(payload)+1]);
    		break;
    	case TATU_FLOW:
    		tatu_flow(&payload[strlen(payload)+1]);
    		break;
    }

    /*
    	Once the requisiton has been properly treated, the body is generated
        the value added to @aux represents the string size
    */

    /* 
    	Adds METHOD PROPERTY and it's value

    	<example>
    		"METHOD":"GET"
    	</example>
    */
    cpyStrConstant(OUT_STR, method_str);
    aux += 9;
    //	it's value
    QUOTE;
    switch(requisition->cmd.OBJ.TYPE){
        case TATU_GET:
            cpyStrConstant(OUT_STR, method_get);
            aux += 3;
            break;
        case TATU_SET:
            cpyStrConstant(OUT_STR, method_set);
            aux += 3;
            break;
    }
    QUOTE; COMMA;

    /* 
    	Inserts the body property: ( "BODY":{ )
    */
    cpyStrConstant(OUT_STR, body_str);
    aux += 8;

    /* 
        Copies the variable of the message that comes in the payload
        <example>
            incoming message: GET STATE lamp
            adds the following json property "lamp":
        </example>
    */
    QUOTE; strcpy(OUT_STR, payload); aux += strlen(payload); QUOTE; COLON;
	
    /*
        Checks if has ocurred a error when trying to attend the request
        
        If there is a error atrributes the false value to the property 
        and closes the message

        <example>
            "lamp":false
        <example> 
    */
    if(requisition->cmd.OBJ.ERROR){ 
        return error_message(aux);
    }

    /*
        If the only return needed is if it was sucessful or not
        and its was a succes atrributes the true value to the property

        <example>
            "airConditioner":true
        <example> 
    */
    if(requisition->cmd.OBJ.TYPE != TATU_GET){
        return sucess_message(aux);  
    }

    /*
        if a more suitable answer is needed, the type must be treated

        <example>
            requisition: GET INT temperatureSensor
            response: "temperatureSensor":25
        </example>
    */
    switch(requisition->cmd.OBJ.CODE) {
        case TATU_CODE_FLOW:
            strcpy(OUT_STR, buffer);
            aux+=strlen(buffer);
            break;
        case TATU_CODE_STR:    
            QUOTE; strcpy(OUT_STR, buffer); aux+=strlen(buffer); QUOTE;
            
            //debugln(RESPONSE_TYPE_INFO);
            break;
        case TATU_CODE_INT:
            itoa((int)buffer,buffer,10);
            strcpy(OUT_STR, buffer);
            aux+=strlen(buffer);
            
            //debugln(RESPONSE_TYPE_VALUE);
            break;
        case TATU_CODE_BOOL:
            if ((bool)buffer)  cpyStrConstant(buffer, true_str);
            else cpyStrConstant(buffer, false_str);
            strcpy(OUT_STR, buffer);
            aux+=strlen(buffer);
            
            //debugln(RESPONSE_TYPE_STATE);
            break;
    }
    
    // //Debug that shows the response returned by @get_funcion
    //debug(THE_RESPONSE);
    //debugln(str_buffer);
    
    /*
        the last part closes the json message:
        "BODY":{"temperatureSensor":32 + }}
    */
    BRACE_RIGHT; BRACE_RIGHT;
    CLOSE_MSG;
    #endif
    
    //debugln(BODY_GENERATED);
    
}

/* Function to abstract some low-level publishing action */
void Device::callback(char *topic, byte *payload, unsigned int length){
    /* Generate the body */
    generateBody((char *) payload, (uint8_t) length);

    if(requisition->cmd.OBJ.TYPE == COMMAND_POST){
        //debugln(IS_A_POST);
        return;
    }
    
    //<//debug>
    //debug(PUBLISHING);
    //debugln(output_message);
    //<///debug>

    cpyStrConstant(&aux_topic_name[len_name],res_str);
    
    //publish the message
    pub(aux_topic_name, output_message);

    aux_topic_name[len_name] = 0;

    // <//debug>
    //debugln(PUBLISHED);
    // <///debug>
}

/*
    Here we define the //debug/print interface using polymorphism
*/
/*void //debug(char* msg){
#ifdef //DEBUG
    Serial.print(msg);
#endif
}
void //debug(const char str[] PROGMEM){
#ifdef //DEBUG
    SerialPrint_PROGMEM(str);
    Serial.println();
#endif
}*/
/*
    The "break-line" prints
*/
/*void //debugln(char* msg){
#ifdef //DEBUG
    Serial.println(msg);
#endif
}
void //debugln(const char str[] PROGMEM){
#ifdef //DEBUG
    SerialPrint_PROGMEM(str);
    Serial.println();
#endif
}*/

void Device::sucess_message(int aux){
     //debugln(NOT_A_GET);
    
    cpyStrConstant(OUT_STR, true_str);
    aux += 4;

    // Closes the message
    BRACE_RIGHT; BRACE_RIGHT;
    CLOSE_MSG;
    return;
}

void Device::error_message(int aux){
    //debugln(PARAM_ERROR);

    cpyStrConstant(OUT_STR, null_str);
    aux += 4;

    // Closes the message
    BRACE_RIGHT; BRACE_RIGHT;
    CLOSE_MSG;
    return;
}
void Device::tatu_get(void* buffer){
    requisition->cmd.OBJ.ERROR = !get_function(requisition->str_hash,buffer,requisition->cmd.OBJ.CODE);
}
void Device::tatu_set(void* request){
    requisition->cmd.OBJ.ERROR = !set_function(requisition->str_hash, requisition->cmd.OBJ.CODE, request);
}
void Device::tatu_flow(void* request){
	requisition->cmd.OBJ.ERROR = !flow_function(requisition->str_hash,requisition->cmd.OBJ.CODE,request);
}
int foo(){
    return 3;
}
void cpyStrConstant (char* destination, const char* source ){
    strcpy(destination,source);
}
