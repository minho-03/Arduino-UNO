/******************************************************************************
 * step4_servo_mqtt.ino
 * 4단계: 서보모터 제어 - MQTT 명령 수신 (Subscribe)
 *
 * 목표: MQTT 구독자(Subscriber)로서 명령을 받아 서보모터를 제어
 *       이전 단계(Publish)와 반대 방향 데이터 흐름
 * 토픽: device/{student_id}/servo/cmd
 * QoS:  Subscribe QoS 1 (PubSubClient 최대치)
 *       개념적으로는 QoS 2가 필요 (Exactly Once)
 *
 * 배선:
 *   SG90 빨간색 (VCC)    → NodeMCU Vin (5V) 또는 외부 전원
 *   SG90 갈색  (GND)     → NodeMCU GND
 *   SG90 주황색 (Signal)  → NodeMCU D5 (GPIO 14)
 *
 * 명령 전송 (라즈베리파이에서):
 *   mosquitto_pub -t "device/01/servo/cmd" -m "OPEN"
 *   mosquitto_pub -t "device/01/servo/cmd" -m "CLOSE"
 *   mosquitto_pub -t "device/01/servo/cmd" -m "45"
 ******************************************************************************/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

// ===== Wi-Fi 설정 =====
const char *ssid     = "5층";          // Wi-Fi 이름
const char *password = "48864886";     // Wi-Fi 비밀번호

// ===== MQTT 설정 =====
const char *MQTT_ID = "servo_01";               
const char *TOPIC   = "device/07/servo/cmd";    

IPAddress broker(192, 168, 0, 75);   // 라즈베리파이 IP
const int MQTT_PORT = 1883;

WiFiClient wclient;
PubSubClient client(wclient);

// ===== 서보모터 설정 =====
const int SERVO_PIN = 14;   // D5 (GPIO 14)
Servo myServo;


// ================= Wi-Fi 연결 =================
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


// ================= MQTT 메시지 수신 콜백 =================
void callback(char* topic, byte* payload, unsigned int length) {

  // payload → 문자열 변환
  char message[length + 1];
  for (unsigned int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';

  Serial.print("수신: [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(message);

  String cmd = String(message);

  if (cmd == "OPEN") {
    myServo.write(90);
    Serial.println("→ 서보 90도 (OPEN)");
  }
  else if (cmd == "CLOSE") {
    myServo.write(0);
    Serial.println("→ 서보 0도 (CLOSE)");
  }
  else {
    int angle = cmd.toInt();
    if (angle >= 0 && angle <= 180) {
      myServo.write(angle);
      Serial.print("→ 서보 ");
      Serial.print(angle);
      Serial.println("도");
    } else {
      Serial.println("잘못된 각도 값");
    }
  }
}


// ================= MQTT 재연결 =================
void reconnect() {
  while (!client.connected()) {

    Serial.print("Attempting MQTT connection...");

    if (client.connect(MQTT_ID)) {
      Serial.println("connected");

      // Subscribe (QoS 1)
      client.subscribe(TOPIC, 1);

      Serial.print("Subscribed to: ");
      Serial.println(TOPIC);
      Serial.println();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


void setup() {
  Serial.begin(115200);

  // ===== 서보 초기화 =====
  myServo.attach(SERVO_PIN);
  myServo.write(0);   // 초기 위치

  delay(100);
  setup_wifi();

  client.setServer(broker, MQTT_PORT);

  // ===== 콜백 등록 =====
  client.setCallback(callback);

  Serial.println("4단계: 서보모터 MQTT 제어");
  Serial.println("=========================");
  Serial.println("명령 대기 중...");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();   // 메시지 수신 대기
}