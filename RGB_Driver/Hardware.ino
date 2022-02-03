

void RGB_Init()
{
  pinMode(RGB_Pins[R_Order-1], OUTPUT);
  pinMode(RGB_Pins[G_Order-1], OUTPUT);
  pinMode(RGB_Pins[B_Order-1], OUTPUT);
  pinMode(LED_PIN, OUTPUT); 
  pinMode(USB1_Pin, OUTPUT);
  pinMode(USB2_Pin, OUTPUT);  
  
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(USB1_Pin, LOW);
  digitalWrite(USB2_Pin, LOW);

  //Default value
  showAnalogRGB( CRGB::Black );
  Current_RGB.r = 255;
  Current_RGB.g = 127;
  Current_RGB.b = 0;
  Intensity = 100;
}








void Set_RGB_Driver_Mode(int Mode, const CRGB& CMD_RGB)
{
  Serial.print("Set_RGB - Mode/Intensity/R/G/B:");
  Serial.print(Mode);
  Serial.print(' ');
  Serial.print(Intensity);
  Serial.print(' ');
  Serial.print(CMD_RGB.r);
  Serial.print(' ');
  Serial.print(CMD_RGB.g);
  Serial.print(' ');
  Serial.println(CMD_RGB.b);  


  switch(Mode)
  {
    case 0: // Switch off
        RGB_Driver_State = 0;
        showAnalogRGB(CRGB::Black);
        lastrun_RainbowCycle = 0;
        Blynk.virtualWrite(V2, 0);
        Blynk.virtualWrite(V3, 0);
        MQTT_publish_RGB_State();
        //Todo Alexa feedback 
        break;

    case 1: // Custom color mode - rgb cmd        
        RGB_Driver_State = 1;
        
        New_RGB.r = CMD_RGB.r ;
        New_RGB.g = CMD_RGB.g ;
        New_RGB.b = CMD_RGB.b ;

        showAnalogRGB(New_RGB);
        Blynk.virtualWrite(V2, 1);
        Blynk.virtualWrite(V3, 0);
        MQTT_publish_RGB_State();
        MQTT_publish_RGB_Color(Current_RGB);
        //Todo Alexa feedback 
        break;

    case 2: // Rainbow mode - transition through colors
        RGB_Driver_State = 2;
        CycleColors();
        lastrun_RainbowCycle = 0;        
        Blynk.virtualWrite(V2, 1);
        Blynk.virtualWrite(V3, 1);
        MQTT_publish_RGB_State();
        //MQTT_publish_RGB_Color(Current_RGB);
        //Todo Alexa feedback
        break;
        
    case 3: 
        break;

    case 4: // Test RGB LED Sequence
        colorBars_Test();        
        Set_RGB_Driver_Mode(0, Current_RGB);
        break;

    case 100: // Intensity change
        if(RGB_Driver_State == 1)
        {
          
          New_RGB.r = Current_RGB.r ;
          New_RGB.g = Current_RGB.g ;
          New_RGB.b = Current_RGB.b ;
          Set_RGB_Driver_Mode(1, New_RGB);
        }
        Blynk.virtualWrite(V1, Intensity);
        MQTT_publish_RGB_Brightness();
        break;
  }
}


void RGB_PinOrder_EEPROM_READ()
{
  EEPROM.begin(512);
  int EEPROM_Addr=21;

  R_Order = EEPROM.read(EEPROM_Addr);   EEPROM_Addr++;
  G_Order = EEPROM.read(EEPROM_Addr);   EEPROM_Addr++;
  B_Order = EEPROM.read(EEPROM_Addr);
}


void RGB_PinOrder_EEPROM_WRITE(char color, int value)
{
  EEPROM.begin(512);
  int EEPROM_Addr;

  if(color == 'r' || color == 'R') 
  		EEPROM_Addr = 21;
  if(color == 'g' || color == 'G') 
  		EEPROM_Addr = 22;
  if(color == 'b' || color == 'B') 
  		EEPROM_Addr = 23;   
  
  EEPROM.write(EEPROM_Addr, value);
  EEPROM.commit(); 
  EEPROM.end();
}



void showAnalogRGB(const CRGB& rgb)
{
  // Intensity Adjustment
  int Min_Intensity = 75; // LEDs don't turn on below this.

  int intensity_calc = map(Intensity, 0, 100, Min_Intensity, 100);

  int red   = rgb.r * intensity_calc/100;
  int green = rgb.g * intensity_calc/100;
  int blue  = rgb.b * intensity_calc/100;


  // Maped to ESP's 1023 PWM output & reversed circuit logic.
  Red_Val    = map(red, 0,255,1023,0);
  Green_Val  = map(green, 0,255,1023,0);
  Blue_Val   = map(blue, 0,255,1023,0);

  analogWrite(RGB_Pins[R_Order-1], Red_Val);
  analogWrite(RGB_Pins[G_Order-1], Green_Val);
  analogWrite(RGB_Pins[B_Order-1], Blue_Val);
}


void CycleColors()
{  
  if(millis() - lastrun_RainbowCycle > RainbowCycleTime || lastrun_RainbowCycle == 0)
  {
    CycleColor_Hue    = CycleColor_Hue + 1;
    int RGB_Intensity = map(Intensity,0,100,0,255);  
    Current_RGB = CHSV( CycleColor_Hue, 255, RGB_Intensity);
    showAnalogRGB(Current_RGB);
    lastrun_RainbowCycle = millis();
  }
}


// Test function to check LED order
void colorBars_Test()
{
  showAnalogRGB( CRGB::Black ); delay(1000);
  showAnalogRGB( CRGB::Red );   delay(1000);
  showAnalogRGB( CRGB::Green ); delay(1000);
  showAnalogRGB( CRGB::Blue );  delay(1000);
  showAnalogRGB( CRGB::Black ); delay(1000);
}





void RGB_Test_Full_Intensity0()
{
  digitalWrite(RGB_Pins[R_Order-1], HIGH);
  digitalWrite(RGB_Pins[G_Order-1], HIGH);
  digitalWrite(RGB_Pins[B_Order-1], HIGH);  
}


void RGB_Test_Full_Intensity1()
{
  digitalWrite(RGB_Pins[R_Order-1], LOW);
  digitalWrite(RGB_Pins[G_Order-1], HIGH);
  digitalWrite(RGB_Pins[B_Order-1], HIGH);  
}


void RGB_Test_Full_Intensity2()
{
  digitalWrite(RGB_Pins[R_Order-1], HIGH);
  digitalWrite(RGB_Pins[G_Order-1], LOW);
  digitalWrite(RGB_Pins[B_Order-1], HIGH);  
}


void RGB_Test_Full_Intensity3()
{
  digitalWrite(RGB_Pins[R_Order-1], HIGH);
  digitalWrite(RGB_Pins[G_Order-1], HIGH);
  digitalWrite(RGB_Pins[B_Order-1], LOW);  
}

