/******************************************************************************
 * step3_temperature_mqtt.ino
 * 3단계: 온도 센서 조건부 퍼블리시 (QoS 0)
 *
 * 목표: 온도가 특정 조건을 만족할 때만 MQTT 메시지를 전송
 *       불필요한 전송을 줄여 네트워크 트래픽 절약
 * 토픽: device/{student_id}/temperature
 * QoS:  0 (주기적 데이터, 유실 허용)
 *
 * 배선:
 *   DHT11 VCC   → NodeMCU 3.3V
 *   DHT11 GND   → NodeMCU GND
 *   DHT11 DATA  → NodeMCU D4 (GPIO 2) + 10kΩ 풀업 저항
 *
 * 라이브러리 설치:
 *   Arduino IDE → 라이브러리 관리자 → "DHT sensor library" (Adafruit) 설치
 *   의존성 "Adafruit Unified Sensor"도 함께 설치
 *
 * 확인: 라즈베리파이에서
 *   mosquitto_sub -t "device/01/temperature" -v
 ******************************************************************************/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// ===== Wi-Fi 설정 =====
const char *ssid     = "5층";
const char *password = "48864886";

// ===== MQTT 설정 =====
const char *MQTT_ID = "temperature_01";
const char *TOPIC   = "device/07/temperature";

IPAddress broker(192, 168, 0, 75);   // 라즈베리파이 IP
const int MQTT_PORT = 1883;

WiFiClient wclient;
PubSubClient client(wclient);

// ===== DHT 센서 설정 =====
#define DHT_PIN   2          // D4 (GPIO 2)
#define DHT_TYPE  DHT11
DHT dht(DHT_PIN, DHT_TYPE);

// ===== 타이밍 =====
unsigned long previousMillis = 0;
const long interval = 2000;   // 2초마다 측정

// ===== 조건 설정 =====
const float THRESHOLD_TEMP = 40.0;     // 방식 A
const float CHANGE_THRESHOLD = 1.0;    // 방식 B
float lastSentTemp = -999.0;


// ================= WiFi =================
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

// ================= MQTT =================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (client.connect(MQTT_ID)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


// ================= 조건부 판단 =================
bool shouldPublish(float temperature) {

  // ====== 방식 A : 임계값 기반 ======
  /*
  if (temperature >= THRESHOLD_TEMP) {
    return true;
  }
  */

  // ====== 방식 B : 변화폭 기반 ======
  
  if (abs(temperature - lastSentTemp) >= CHANGE_THRESHOLD) {
    lastSentTemp = temperature;
    return true;
  }
  

  return false;
}


void setup() {
  Serial.begin(115200);
  dht.begin();

  delay(100);
  setup_wifi();
  client.setServer(broker, MQTT_PORT);

  Serial.println("3단계: 온도 조건부 MQTT 퍼블리시");
  Serial.println("=================================");
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float temperature = dht.readTemperature();

    if (isnan(temperature)) {
      Serial.println("DHT 읽기 실패!");
      return;
    }

    if (shouldPublish(temperature)) {

      char tempStr[10];
      dtostrf(temperature, 4, 2, tempStr);

      client.publish(TOPIC, tempStr);  // QoS 0

      Serial.print("[전송] ");
      Serial.print(TOPIC);
      Serial.print(" => ");
      Serial.println(tempStr);

    } else {

      Serial.print("[스킵] 온도: ");
      Serial.print(temperature);
      Serial.println(" ℃ - 조건 미충족");
    }
  }
}