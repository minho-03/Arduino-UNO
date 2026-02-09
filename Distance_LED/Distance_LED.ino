int ledPin = 13;
int trigPin = 9;
int echoPin = 10;

long duration;
int distance;

void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // 반사파 시간 측정
  duration = pulseIn(echoPin, HIGH);

  // 거리 계산 (cm)
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // 거리값으로 LED 깜빡임 속도 제어
  digitalWrite(ledPin, HIGH);
  delay(distance * 5);   // 거리가 멀수록 느리게
  digitalWrite(ledPin, LOW);
  delay(distance * 5);
}
