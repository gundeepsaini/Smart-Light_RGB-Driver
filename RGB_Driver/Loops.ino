


void Always_loop()
{
  OTA_Check();
}



void Fast_Loop()
{   
  Blynk.run();
  server.handleClient();
  MQTT_loop();
}



void Mid1_Loop()
{ 
  if(RGB_Driver_State == 2)
    CycleColors();
}


void Mid2_Loop()
{
  RunOnce_30mins();
}
 


void Slow_Loop()
{
  MQTT_publish();
}


void VerySlow_Loop()
{

  
}



void RunOnce_30mins()
{
	if(millis()/1000 - lastrun_30mins > 30 * 60 || lastrun_30mins == 0)
	{
    lastrun_30mins = millis()/1000;

    
    Time_NTP_Update();   
	}
}
