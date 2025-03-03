#include <WiFi.h>
#include "ThingSpeak.h" 
#include <HTTPClient.h>

WiFiClient client;
const int LED = 32;
const char *ssid = "xxxxxx";
const char *password = "xxxxxxx";
unsigned long myChannelNumber = xxxxxxx;
const char *myWriteAPIKey = "xxxxxxxx";
const char *apiKey = "xxxxxxxx";
const char *server = "http://api.thingspeak.com";
const int channelID = xxxxxxx;

void setup() {
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);

    WiFi.mode(WIFI_STA);
    ThingSpeak.begin(client);
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi...");

    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected");
        delay(500);
    }
    digitalWrite(LED, LOW);
    Serial.println("WiFi connected!");

}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        // อ่านข้อมูลจาก ThingSpeak
        HTTPClient http;
        String url = String(server) + "/channels/" + String(channelID) + "/fields/3/last?api_key=" + apiKey;
        http.begin(url);
        
        int httpResponseCode = http.GET();
        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.print("Received data from Field 3: ");
            Serial.println(response);
        } else {
            Serial.print("Error on HTTP request: ");
            Serial.println(httpResponseCode);
        }
        http.end();
    }
}
