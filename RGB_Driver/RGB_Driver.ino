/*-----------------------------------------

            SMART RGB LED STRIP DRIVER

Origin date : 2017 (v00)
Device		  : ESP8266 NodeMcU

 - Transistor/Mosfet based freq control of RGB LED

References:
  https://github.com/FastLED/FastLED/wiki/

Note:
  - OTA: Reset ESP after upload, disable IPv6, 
      reset adapter, allow port in firewall
  - OTA: send "OTA" on blynk terminal to enter dedicated mode
    or navigate to "ip/OTA" for OTA through web portal
  

------------------------------------------- */



/* ------------- LIB ------------------------------ */

#include "Secrets.h"

#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>

#include <BlynkSimpleEsp8266.h>
#include <FastLED.h>






/* ------------- CONFIG VAR ------------------------------ */
#define SERIALCOM true
#define Button_PIN D0
#define LED_PIN   D3
#define USB1_Pin  D1
#define USB2_Pin  D2

int RGB_Pins[] = {12,13,14};    // R,G,B   //D6=12, D7=13, D5=14
int R_Order=1, G_Order=2, B_Order=3;

int RainbowCycleTime = 500;   // in ms
unsigned long looptime_Fast     = 0;    // in secs
unsigned long looptime_Mid1     = 0;    // in secs
unsigned long looptime_Mid2     = 10;   // in secs
unsigned long looptime_Slow 		  = 1 * (60);   	 // in mins
unsigned long looptime_VerySlow 	= 1 * (60*60);   // in hours


/* ------------- VAR ------------------------------ */
const char* ssid 		         = SECRET_WIFI_SSID3;
const char* pass 		         = SECRET_WIFI_PASS3;
const char* DeviceHostName   = SECRET_Device_Name5;
const char* OTA_Password 	   = SECRET_Device_OTA_PASS; 
unsigned long lastrun_fast, lastrun_Mid1;
unsigned long lastrun_Mid2, lastrun_slow, lastrun_Veryslow;
unsigned long lastrun_30mins,lastrun_OTA, lastrun_RainbowCycle;
bool OTA_Mode=0, USB1_State, USB2_State;
uint8_t CycleColor_Hue;
int Intensity=100;
int Red_Val, Green_Val, Blue_Val;
int RGB_Driver_State = 0;   // Modes: 0-Off, 1-Custom RGB Color, 2-Move through Rainbow, 3-Fixed Color
CRGB Current_RGB, New_RGB;
ESP8266WebServer server(80);
String webpage ="";



/* ------------- Functions ------------------------------ */
void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  if(SERIALCOM)
  {
  	Serial.begin(115200);
  	Serial.println(DeviceHostName);
  }

  wifi_station_set_hostname(DeviceHostName);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
  	if(SERIALCOM)
    	Serial.println("Wifi Error! Rebooting in 30s...");
    delay(30 *1000);
    ESP.restart();
    while(1);
  }
  
  if(SERIALCOM)
  {
	  Serial.print("Connected to ");
  	Serial.print(WiFi.SSID());
  	Serial.print(", IP:");
  	Serial.println(WiFi.localIP());
  }
    
  //RGB_PinOrder_EEPROM_READ();
  RGB_Init();
  Blynk_Config();
  OTA_Config();
  WebServer_Config();
  MQTT_config();
  Config_Time();

  digitalWrite(LED_BUILTIN, HIGH);
  if(SERIALCOM)
  	Serial.println("Ready");
}



void loop() 
{
  // Always
  Always_loop();

  // Fast Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_fast > looptime_Fast) || lastrun_fast ==0))
  {
    lastrun_fast = millis()/1000;
    Fast_Loop();
  }

  // Mid1 Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Mid1 > looptime_Mid1) || lastrun_Mid1 ==0))
  {
    lastrun_Mid1 = millis()/1000;
    Mid1_Loop();
  }

  // Mid2 Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Mid2 > looptime_Mid2) || lastrun_Mid2 ==0))
  {
    lastrun_Mid2 = millis()/1000;
    Mid2_Loop();
  }

  // Slow Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_slow > looptime_Slow) || lastrun_slow ==0))
  {
    lastrun_slow = millis()/1000;
    Slow_Loop();
  }

    // Very Slow Loop
 if(!OTA_Mode && ((millis()/1000 - lastrun_Veryslow > looptime_VerySlow) || lastrun_Veryslow ==0))
  {
    lastrun_Veryslow = millis()/1000;
    VerySlow_Loop();
  }
}
