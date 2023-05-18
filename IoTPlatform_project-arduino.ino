#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#define DHTTYPE DHT11
const int DHTPin = 5;   //D1
DHT dht(DHTPin, DHTTYPE);
const char* ssid = "d2-539";
const char* password = "d2539515";
const char* mqtt_server = "127.0.0.1";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.publish("outTopic", "hello world");
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void loop() {
  delay(2000);
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  
  if (now - lastMsg > 6000) {
    lastMsg = now;
    int chk = DHT.read11(DHT11_PIN);
  }
  char message[58];
  char message2[58];
  String  msg= "" ;
  msg = msg + DHT.temperature;
  msg.toCharArray(message,58);
  String msg2 = "";
  msg2 = msg2 + DHT.humidity;
  msg2.toCharArray(message2,58);
      
  Serial.println(message);
  Serial.println(message2);
    
  client.publish("temperature", message);
  client.publish("humidity",message2);
}
