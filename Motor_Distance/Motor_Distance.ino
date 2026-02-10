int AA = 6;       // 모터 A
int AB = 7;       // 모터 B

int trigPin = 9;  // 초음파 TRIG
int echoPin = 10; // 초음파 ECHO

long duration;
int distance;

void setup() {
  pinMode(AA, OUTPUT);
  pinMode(AB, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  // 초음파 발사
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // 반사 시간 측정
  duration = pulseIn(echoPin, HIGH);

  // 거리 계산 (cm)
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 15) {
    // 장애물 없음 → 정회전
    digitalWrite(AA, HIGH);
    analogWrite(AA, 80);
    digitalWrite(AB, LOW);
  } else {
    // 장애물 감지
    // 정지
    digitalWrite(AA, LOW);
    digitalWrite(AB, LOW);
    delay(1000);

    // 역회전
    digitalWrite(AA, LOW);
    analogWrite(AA, 80);
    digitalWrite(AB, HIGH);
    delay(1000);
  }

  delay(100);
}
