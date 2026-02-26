#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// === wifi 설정 ===
const char *ssid = "5층";
const char *password = "48864886";

IPAddress broker(192,168,0,75);
const char *ID = "ESP8266_Ultrasonic";
const char *TOPIC = "device/07/ultrasonic";

#define TRIG_PIN D1   
#define ECHO_PIN D2   

WiFiClient wclient;
PubSubClient client(wclient);

void setup_wifi() {
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(ID)) {
      Serial.println("connected");
    } else {
      Serial.println(" failed, try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  setup_wifi();
  client.setServer(broker, 1883);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  char msg[10];
  dtostrf(distance, 4, 2, msg);

  client.publish(TOPIC, msg);
  Serial.println(String(TOPIC) + " => " + msg);

  delay(2000);  
}