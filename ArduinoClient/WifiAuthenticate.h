#ifndef _WIFICONNECT_H    // Put these two lines at the top of your file.
#define _WIFICONNECT_H    // (Use a suitable name, usually based on the file name.)

// Place your main header code here.

#include <StreamString.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Pinger.h>

String myIpAddress = "";
String myMacAddress = "";

String metaRedirUrl = "";
String referer = "";
String postData = "";
int numHeaders = 1;
const char* HeaderKeys [] = {"Auth"};
String AuthValue = "1234";

unsigned long delayTime = 10;

String host = "google.com";
const char* wifiHost = "wifi.cos.edu";
const char* finalHost = "example.com";
const int httpPort = 80;

// Removes end line characters
void SanitizeString(String& str)
{
  String res = "";
  for(int x = 0; x < str.length(); x++)
  {
    if(str[x] != '\n')
      res = res + str[x];
  }

  str = res;
}

// Gets blocking user input
String GetUserInput()
{
  String result = "";

  do{
    result = Serial.readString();
  }while(result == "");

  SanitizeString(result);
  return result;
}

// Prints connection status based on code given by WiFi
void PrintConnectionStatus(int code)
{
  switch(code)
  {
    case WL_CONNECTED:
      Serial.println("Connected");
      break;
      
    case WL_NO_SHIELD:
      Serial.println("No Shield");
      break;

    case WL_IDLE_STATUS:
      Serial.println("Idle");
      break;

    case WL_NO_SSID_AVAIL:
      Serial.println("No SSID");
      break;

    case WL_SCAN_COMPLETED:
      Serial.println("Scan Complete");
      break;

    case WL_CONNECT_FAILED:
      Serial.println("Connection Failed");
      break;

    case WL_CONNECTION_LOST:
      Serial.println("Connection Lost");
      break;

    case WL_DISCONNECTED:
      Serial.println("Connecting...");
      break;
  }
}

// Prints Encryption based on code given by WiFi
void PrintEncryptionType(int code)
{
  switch(code)
  {
    case ENC_TYPE_WEP:
      Serial.println("WEP");
      break;

    case ENC_TYPE_TKIP:
      Serial.println("WPA");
      break;

    case ENC_TYPE_CCMP:
      Serial.println("WPA2");
      break;

    case ENC_TYPE_NONE:
      Serial.println("None");
      break;

    case ENC_TYPE_AUTO:
      Serial.println("Auto");
      break;
  }
}

// List all available networks
void ListNetworks()
{
  //Scan for nearby networks
  Serial.println("<ListNetworks>: Scanning for all available networks:");
  int count = WiFi.scanNetworks();
  if(count == -1)
  {
    Serial.println("None found");
  }

  // Print out found SSIDs
  for(int index = 0; index < count; index++)
  {
    Serial.print(WiFi.SSID(index) + ": ");
    PrintEncryptionType(WiFi.encryptionType(index));
  }
}

// Initialises the WiFi module
bool BeginConnection() {
  // myIpAddress = WiFi.localIP().toString();
  // myMacAddress = WiFi.macAddress();
 // Serial.println("<InitializeConnectionEst>: Setting up wifi");

  // if(WiFi.status() == WL_NO_SHIELD)
  // {
  //   Serial.println("No Wifi shield was found");
  // }
 
  // ListNetworks();
  // Serial.println("/////////////////////////////");

   // Get connection info
  // Serial.print("Please enter in an SSID: ");
  // String ssid = GetUserInput();
  // Serial.println(ssid);

PASSWORD_INPUT:

  // Serial.print("Please enter in the password (Type -none if no password is needed): ");
  // String pw = GetUserInput();
  // if(pw == "-none")
  // {
//     pw = "";
 //  }
  
  // for(int x = 0; x < pw.length(); x++)
    // Serial.print("*");

 //  Serial.println();
//   Serial.println("/////////////////////////////");

  // Begin connection
 //  Serial.println("<InitializeConnectionEst>: Connecting to: " + ssid);
  // WiFi.begin(&ssid[0], &pw[0]);
  WiFi.begin("COS_PUB", "");

  // Stop trying if an error orrurs or it connects
  while (!(WiFi.status() == WL_CONNECTED || WiFi.status() == WL_CONNECT_FAILED || WiFi.status() == WL_NO_SSID_AVAIL))
  {
    delay(500);
    Serial.print("<InitializeConnectionEst>: Status: ");
    PrintConnectionStatus(WiFi.status());
  }

  // Try another password?
  if(WiFi.status() == WL_CONNECT_FAILED)
  {
    Serial.println("<InitializeConnectionEst>: The password was invalid. Would you like to try again?: ");
    String ans = GetUserInput();
    ans.toLowerCase();

    if(ans == "y" || ans == "yes")
    {
      goto PASSWORD_INPUT;
    }
    else
    {
      return false;
    }
    
  }
  else if (WiFi.status() == WL_NO_SSID_AVAIL)
  {
    Serial.println("<InitializeConnectionEst>: The SSID you typed in was not found");
    return false;
  }

  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("<InitializeConnectionEst>: Connection Established");
  Pinger pinger;
  pinger.Ping("http://cos-ar.herokuapp.com/AHub");
  
  return true;
}

// Gets info from a given url
bool GetFromServer(String ServerURL, String& Result, bool& bIsCaptive)
{
  HTTPClient Client;
	bool Success = false;
	Serial.println("<SendPacket>: Sending packet...");

	// Send Login Packet
	Client.begin(ServerURL);
	Client.addHeader("Content-Type", "application/json");
  Client.collectHeaders(HeaderKeys, numHeaders);

	int code = Client.GET();

	if (code > 0)
	{
    Serial.println("<SendPacket>: Request code recieved: " + String(code));
		Success = true;
		String value = Client.getString();
    String authCode = Client.header(HeaderKeys[0]);
    Client.end();
    Serial.println("<SendPacket>: <AuthCode>: " + authCode);

		// The server we connected is our own since we got back what we are expecting
		if (authCode == AuthValue)
		{
			bIsCaptive = false;
      Result = value;
		}
    else if (code == 503 || code == 400 || code == 404)
    {
      bIsCaptive = false;
      Serial.println("<SendPacket>: Server could not be reached!");
    }
		else
		{
			bIsCaptive = true;
		}
	}

	return Success;
}

void PassThroughCaptive() {
  HTTPClient Client;
  String url = "http://wifi.cos.edu/auth/index.html/u";
  Client.begin(url);
  Client.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String payload = "email=FNL%40cos.edu&cmd=authenticate&Login=I+ACCEPT";
  Client.POST(payload);

  Serial.println("<PassThroughCaptive>: Sent login to captive");
  String value = Client.getString();
  Client.end();
  delay(1000);
}

#endif _WIFICONNECT_H
