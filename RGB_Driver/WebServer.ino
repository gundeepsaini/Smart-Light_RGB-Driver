

void WebServer_Config()
{
  
  server.onNotFound(handleNotFound);
  server.on("/",Webpage_Home);
  server.on("/OTA",Webpage_OTA);
  server.on("/restart",Webpage_restart);
  
  server.begin();  
}

void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found! Send one of these: /OTA, /restart");
}

void Webpage_Home()
{
  Prep_webpage();
  server.send ( 200, "text/html", webpage);
  delay(10);
  webpage = "";
}

void Webpage_OTA()
{
  OTA_Mode = 1;
  server.send(200,"text/plain","OTA mode active now");
}


void Webpage_restart()
{
  server.send(200,"text/plain","Restarting ESP");
  if(SERIALCOM)
    Serial.println("Restart requested from webserver");
  
  delay(1000);
  ESP.restart();
  delay(1000);
  while(1);
}



