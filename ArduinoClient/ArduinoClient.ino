#include "WifiAuthenticate.h"
#include "fancontrol.h"
#define D7 13
#define D6 12
#define D5 14
#define D8 15
#define OnBoardLED 16

// Reset Function
void resetDevice() {
  while(true);
}

void setup() 
{
  pinMode(D7,OUTPUT);
  pinMode(D6,OUTPUT);
  pinMode(D8,OUTPUT);
  pinMode(D5,OUTPUT);
  pinMode(OnBoardLED,OUTPUT);
  Serial.begin(9600);
  while(!Serial);
  delay(100);
  while(!BeginConnection());
}

void loop() {
  
  String Result = "";
  String data = "";
  bool bIsCaptive = false;
  flashLight(OnBoardLED);

  int status = WiFi.status();
  if(status != WL_CONNECTED)
  {
    flashLight(D5);
  }
  
  if(GetFromServer("http://cos-ar.herokuapp.com/FanSpeed", Result, bIsCaptive))
  {
    if(bIsCaptive)
    {
      Serial.println("Passing through captive portal...");
      PassThroughCaptive();
      delay(100);
    }
    else
    {
      flashLight(D6);
      Serial.println("Server Response: " + Result);
      Serial.println();
      setSpeed(Result,D7);
      delay(1000);
    }
  }
  else
  {
    flashLight(D8);
    Serial.println("Connection lost...");
    delay(1000);
  }
}
