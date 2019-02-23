#include "WifiAuthenticate.h"
#include "fancontrol.h"
#define PIN_NUMBER 4

const String SecretKey = "284923401";

// Reset Function
void resetDevice() {
  while(true);
}

void setup() 
{
  Serial.begin(9600);
  while(!Serial);
  delay(100);
  while(!InitializeConnectionEst());
}

void loop() {
  
  String Result = "";
  String data = "";
  String Content = "{\"SecretKey\": " + SecretKey + "\"data\": \"" + data + "\"}";
  bool bIsCaptive = false;
  
  if(SendPacket("http://cos-ar.herokuapp.com/AHub", Content, '!', Result, bIsCaptive))
  {
    if(bIsCaptive)
    {
      Serial.println("The website is a captive portal");
      PassThroughCaptive();
    }
    else
    {
      Serial.println("Server: " + Result);
      Serial.println();
      setSpeed(Result,PIN_NUMBER);
    }
  }
  else
  {
    Serial.println("No internet connection detected! Resetting device...");
    resetDevice();
  }
  
  delay(10000);
}
