#include <WiFi.h>
#include <MQTT.h>

#define BUILTIN_LED 32

// ข้อมูล WiFi และ MQTT Broker
const char* ssid = "xxxxxxx";
const char* password = "xxxxxxx";

const char* mqtt_broker = "xxxxxxx";
const char* topic1 = "xxxxxxx";      // Topic 1
const char* topic2 = "xxxxx";    // Topic 2
const char* mqtt_username = "xxxx";
const char* mqtt_password = "xxxxxx";
const int mqtt_port = 1883;
String client_id = "esp32-subscriber";  // ตั้ง Client ID ใหม่

WiFiClient wifiClient;
MQTTClient client;

void callback(String &topic, String &payload) {
    Serial.print("📢 Message received on topic: ");
    Serial.println(topic);
    Serial.print("📄 Payload: ");
    Serial.println(payload);
    Serial.println("-----------------------");

    // แสดงสถานะอุปกรณ์ ถ้าได้รับจาก "DEVICE_STATUS"
    if (topic == topic2) {
        if (payload == "ON") {
            Serial.println("🔵 Device is ONLINE!");
            digitalWrite(BUILTIN_LED, HIGH);
        } else if (payload == "OFF") {
            Serial.println("⚫ Device is OFFLINE!");
            digitalWrite(BUILTIN_LED, LOW);
        }
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(BUILTIN_LED, OUTPUT);

    WiFi.begin(ssid, password);
    Serial.println("\n🔗 Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\n✅ WiFi Connected!");
    Serial.print("📡 IP Address: ");
    Serial.println(WiFi.localIP());

    client.begin(mqtt_broker, mqtt_port, wifiClient);
    client.onMessage(callback);

    while (!client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
        Serial.print("🔄 Attempting MQTT connection...");
        delay(2000);
    }
    Serial.println("✅ MQTT Broker Connected!");

    // Subscribe ไปยัง 2 Topics
    client.subscribe(topic1, 1);  // QoS 1
    client.subscribe(topic2, 1);
    Serial.println("📌 Subscribed to topics: SENSOR_DATA & DEVICE_STATUS");
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("🔄 Reconnecting to MQTT...");
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("✅ Reconnected!");
            client.subscribe(topic1, 1);
            client.subscribe(topic2, 1);
        } else {
            Serial.println("❌ Reconnect failed, retrying...");
            delay(5000);
        }
    }
}
