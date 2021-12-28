

/******************** LIB **************************************/
#define MQTT_MAX_PACKET_SIZE 1024  // < ----- Change in lib: This is because the defaul maxium length is 128b. So just go to PubSubClient.h and change #define MQTT_MAX_PACKET_SIZE 128 to #define MQTT_MAX_PACKET_SIZE 1024
#include "src/pubsubclient/PubSubClient.h"
//#include <ArduinoJson.h>


/********************* Var *************************************/
WiFiClient espClient;
PubSubClient client(espClient);

long lastReconnectAttempt = 0;


/******************** Config **************************************/
const char* mqtt_server 	  = SECRET_MQTT_Server;
const char* mqtt_user 		  = SECRET_MQTT_User;
const char* mqtt_password 	= SECRET_MQTT_Pass;


#define MQTT_TOPIC_STATE_LIGHT        "HA/RGBdriver/Light/state"
#define MQTT_TOPIC_CMD_LIGHT          "HA/RGBdriver/Light/cmd"
#define MQTT_TOPIC_STATE_BRIGHTNESS   "HA/RGBdriver/Brightness/state"
#define MQTT_TOPIC_CMD_BRIGHTNESS     "HA/RGBdriver/Brightness/cmd"
#define MQTT_TOPIC_STATE_COLOR        "HA/RGBdriver/Color/state"
#define MQTT_TOPIC_CMD_COLOR          "HA/RGBdriver/Color/cmd"

#define LIGHT_ON    "ON"
#define LIGHT_OFF   "OFF"

// Will Topic - Availability
#define MQTT_TOPIC_WILL       "HA/RGBdriver/status"     
#define MQTT_OFFLINE          "Offline"
#define MQTT_ONLINE           "Active"


const uint8_t MSG_BUFFER_SIZE = 20;
char MSG_BUFFER[MSG_BUFFER_SIZE]; 


/**************** External Functions ************************************/

void MQTT_config()
{
	client.setServer(mqtt_server, SECRET_MQTT_Port);
 	client.setCallback(MQTT_MessageRecd_callback);
}


void MQTT_loop()
{
	if (!client.connected())
    	MQTT_reconnect();              
  
  	client.loop();
}


void MQTT_publish()
{   
  MQTT_publish_RGB_State();
}


/**************** Internal Functions ************************************/

void MQTT_reconnect() 
{
  if (millis()/1000 - lastReconnectAttempt > 30 || lastReconnectAttempt == 0) 
  {
      if(SERIALCOM)
        Serial.println("MQTT reconnecting");
      
      //boolean connect (clientID, [username, password], [willTopic, willQoS, willRetain, willMessage], [cleanSession])
      if (client.connect(DeviceHostName, mqtt_user, mqtt_password, MQTT_TOPIC_WILL, 1, true, MQTT_OFFLINE)) 
      {        
        MQTT_publish_RGB_State();
        MQTT_publish_RGB_Brightness();
        MQTT_publish_RGB_Color(Current_RGB);
      
        client.subscribe(MQTT_TOPIC_CMD_LIGHT);
        client.subscribe(MQTT_TOPIC_CMD_BRIGHTNESS);
        client.subscribe(MQTT_TOPIC_CMD_COLOR);

        client.publish(MQTT_TOPIC_WILL, MQTT_ONLINE, true);

        if(SERIALCOM)
            Serial.println("MQTT connected");
      }
      lastReconnectAttempt = millis()/1000;
  }
}




void MQTT_MessageRecd_callback(char* p_topic, byte* p_payload, unsigned int p_length) 
{
  String payload;
  for (uint8_t i = 0; i < p_length; i++) 
    { payload.concat((char)p_payload[i]); }

  if (String(MQTT_TOPIC_CMD_LIGHT).equals(p_topic)) 
  {
    if (payload.equals(String(LIGHT_ON)))    
      {
        if(RGB_Driver_State != 1)
            Set_RGB_Driver_Mode(1, Current_RGB);
      }      
    else 
      if (payload.equals(String(LIGHT_OFF))) 
        Set_RGB_Driver_Mode(0, Current_RGB);
  } 

  else 
  {
    if (String(MQTT_TOPIC_CMD_BRIGHTNESS).equals(p_topic)) 
    {
      uint8_t brightness = payload.toInt();
      if (brightness < 0 || brightness > 100) 
        { return; } 
      else 
      {
        Intensity = brightness;
        Set_RGB_Driver_Mode(100, Current_RGB);
        MQTT_publish_RGB_Brightness();
      }
    } 

    else 
    {
      if (String(MQTT_TOPIC_CMD_COLOR).equals(p_topic)) 
      {
        uint8_t firstIndex = payload.indexOf(',');
        uint8_t lastIndex = payload.lastIndexOf(',');  
        
        uint8_t rgb_red = payload.substring(0, firstIndex).toInt();
        if (rgb_red < 0 || rgb_red > 255) 
          return;
        else 
          Current_RGB.r = rgb_red;      
        
        uint8_t rgb_green = payload.substring(firstIndex + 1, lastIndex).toInt();
        if (rgb_green < 0 || rgb_green > 255) 
          return;      
        else 
          Current_RGB.g = rgb_green;      
        
        uint8_t rgb_blue = payload.substring(lastIndex + 1).toInt();
        if (rgb_blue < 0 || rgb_blue > 255) 
          return;
        else 
          Current_RGB.b = rgb_blue;
        
        Set_RGB_Driver_Mode(1, Current_RGB);
      }
    }
  }
}



bool MQTT_hasValueChanged(float newValue, float prevValue, float maxDiff) 
{
  return !isnan(newValue) && (newValue < prevValue - maxDiff || newValue > prevValue + maxDiff);
}



void MQTT_publish_RGB_State() 
{
  if (RGB_Driver_State == 0) 
    client.publish(MQTT_TOPIC_STATE_LIGHT, LIGHT_OFF, true);
  else 
    client.publish(MQTT_TOPIC_STATE_LIGHT, LIGHT_ON, true);
}


void MQTT_publish_RGB_Brightness() 
{
  snprintf(MSG_BUFFER, MSG_BUFFER_SIZE, "%d", Intensity);
  client.publish(MQTT_TOPIC_STATE_BRIGHTNESS, MSG_BUFFER, true);
}


void MQTT_publish_RGB_Color(const CRGB& rgb) 
{
  snprintf(MSG_BUFFER, MSG_BUFFER_SIZE, "%d,%d,%d", rgb.r, rgb.g, rgb.b);
  client.publish(MQTT_TOPIC_STATE_COLOR, MSG_BUFFER, true);
}