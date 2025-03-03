#include <WiFi.h>
#include "ThingSpeak.h"

WiFiClient  client;
const int LED=32;
const char *ssid = “XXXXX";
const char *password = “YYYYY";
unsigned long myChannelNumber = 000000;
const char * myWriteAPIKey = “NNNNNNNN";

void setup() {
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,HIGH);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  WiFi.begin(ssid,password);
  Serial.println("Connecting Wifi...");
  while(WiFi.status() != WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi not CONNECTED");
    delay(500);
  }
  digitalWrite(LED,LOW);    
  Serial.println ("Wifi CONNECTED");
}

void loop() {
  if(WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED,!digitalRead(LED));    
    ThingSpeak.setField(1, analogRead(A0));
    ThingSpeak.setField(2, hallRead());    
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200){
       Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }    
  }
  else
  { digitalWrite(LED,HIGH);
  }  
  delay(15000);
}
