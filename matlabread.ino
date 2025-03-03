#include <WiFi.h>
#include "ThingSpeak.h"  // ThingSpeak library สำหรับเขียนข้อมูล
#include <HTTPClient.h>  // HTTPClient library สำหรับอ่านข้อมูล

WiFiClient client;
const int LED = 32;
const char *ssid = "xxxxxx";   // เปลี่ยนเป็นชื่อ WiFi ของคุณ
const char *password = "xxxxxxx";  // เปลี่ยนเป็นรหัสผ่าน WiFi ของคุณ
unsigned long myChannelNumber = xxxxxxx;  // Channel ID สำหรับเขียนข้อมูล
const char *myWriteAPIKey = "xxxxxxxx";  // Write API Key
const char *apiKey = "xxxxxxxx";  // Read API Key
const char *server = "http://api.thingspeak.com";
const int channelID = xxxxxxx;  // Channel ID สำหรับอ่านข้อมูล

void setup() {
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);

    WiFi.mode(WIFI_STA);
    ThingSpeak.begin(client);  // Initialize ThingSpeak
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
