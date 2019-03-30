#include "WifiAuthenticate.h"
#include "fancontrol.h"
#define SpeedControl 13
#define L_ResponseObtained 5
#define L_NotConnected 7
#define L_ConnectionLost 6
#define OnBoardLED 2

// Reset Function
void resetDevice() {
  while(true);
}

void setup() 
{
  pinMode(SpeedControl,OUTPUT);
  pinMode(L_ResponseObtained,OUTPUT);
  pinMode(L_ConnectionLost,OUTPUT);
  pinMode(L_NotConnected,OUTPUT);
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
    flashLight(L_NotConnected);
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
      flashLight(L_ResponseObtained);
      Serial.println("Server Response: " + Result);
      Serial.println();
      setSpeed(Result,SpeedControl);
      delay(1000);
    }
  }
  else
  {
    flashLight(L_ConnectionLost);
    Serial.println("Connection lost...");
    delay(1000);
  }
}
