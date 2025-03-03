#include <WiFi.h>
#include <MQTT.h>

#define BUILTIN_LED 32

// Update these with values suitable for your network.
const char* ssid = "xxxxxxx_2.4GHz";
const char* password = "xxxxxxx";

const char* mqtt_broker = "xxxxxxxxx";
const char *topic = "TEST_ESP32";
const char *mqtt_username = "xxxxxxa";
const char *mqtt_password = "xxxx";
const int mqtt_port = 1883;
String client_id = "esp32-client-";

WiFiClient wifiClient;
MQTTClient client;

// Callback to handle incoming messages QoS 1
void callback(String &topic, String &payload) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message: ");
    Serial.println(payload);
    Serial.println("-----------------------");
}

void setup() {
    Serial.begin(115200);

    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.println("");
    Serial.print("Connecting to WiFi ");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);     
    }
    Serial.println("");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  
    // Connect to MQTT broker
    client.begin(mqtt_broker, mqtt_port, wifiClient);
    client.onMessage(callback);
    while (!client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
        Serial.print("Attempting MQTT connection...");
        delay(2000);
    }
    Serial.println("Public emqx mqtt broker connected");

    // Subscribe to a topic with QoS 1
    client.subscribe(topic, 1);  // QoS 1
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    // Read the analog value
    int sensorValue = analogRead(A0);
    
    // Print the sensor value to Serial Monitor
    Serial.print("Sensor Value: ");
    Serial.println(sensorValue);
    
    String payload = String(sensorValue);

    if (client.publish(topic, payload, true, 1)) {  // QoS 1 and retain flag set to true
        Serial.println("Data sent to MQTT broker and retained successfully.");
    } else {
        Serial.println("Failed to send data.");
    }

    delay(1000);  // Reduce the delay to 1 second
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("connected");
            client.subscribe(topic, 1);  // Subscribe with QoS 1
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.connected() ? "Connected" : "Disconnected");
            delay(5000);
        }
    }
}
