

/*--------------------------------------------------------*/


char BlynkAuth[] = SECRET_BLYNK_AUTH5;


/*--------------------------------------------------------*/

void Blynk_Config()
{
    Blynk.config(BlynkAuth);

    while (Blynk.connect(1000) == false);  
    
    //send data to server for sync
    Blynk.virtualWrite(V1, 100); 
    Blynk.virtualWrite(V2, 0);
    Blynk.virtualWrite(V3, 0);
    Blynk.virtualWrite(V4, 0);
    Blynk.virtualWrite(V5, 0);    
 
}


void Blynk_pushDataToGraph()
{

}


/*--------------------------------------------------------*/


// RGB INPUT on Blynk
BLYNK_WRITE(V0)
{
  // get channel value in MERGE mode as R,G,B
  int r = param[0].asInt();
  int g = param[1].asInt();
  int b = param[2].asInt();

  Current_RGB.r = r;
  Current_RGB.g = g;
  Current_RGB.b = b;
  
  if(r==0 && g==0 && b==0)
     Set_RGB_Driver_Mode(0, Current_RGB);
  else
     Set_RGB_Driver_Mode(1, Current_RGB);
      
}


// Intensity Slider
BLYNK_WRITE(V1)
{  
  int value = param.asInt();
  Intensity = value;
  
  Set_RGB_Driver_Mode(100, Current_RGB);
}


BLYNK_WRITE(V2)
{
  // Light On/Off Button
  boolean Light_State = param.asInt();

  if(Light_State)
      Set_RGB_Driver_Mode(2, Current_RGB);     
  else
      Set_RGB_Driver_Mode(0, Current_RGB);
}


BLYNK_WRITE(V3)
{
  // Rainbow On/Off Button
  boolean Rainbow_State = param.asInt();

  if(Rainbow_State)
      Set_RGB_Driver_Mode(2, Current_RGB);     
  else
      Set_RGB_Driver_Mode(0, Current_RGB);
}


BLYNK_WRITE(V4)
{
  // USB1 On/Off Button
  int Value = param.asInt();
  USB1_State = bool(Value);
  digitalWrite(USB1_Pin, USB1_State);
}

BLYNK_WRITE(V5)
{
  // USB2 On/Off Button
  int Value = param.asInt();
  USB2_State = bool(Value);;
  digitalWrite(USB2_Pin, USB2_State);
}

BLYNK_WRITE(V6)
{
    
}

BLYNK_WRITE(V7)
{

}