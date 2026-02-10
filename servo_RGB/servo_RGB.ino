#include <Servo.h>

Servo myservo;

int potPin = A0;
int servoPin = 9;

int redPin = 3;
int greenPin = 5;

int val;      
int angle;    

void setup() {
  Serial.begin(9600);
  myservo.attach(servoPin);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
}

void loop() {
  val = analogRead(potPin);
  angle = map(val, 0, 1023, 0, 180);
  myservo.write(angle);
  Serial.println(angle);
  if (angle >= 30 && angle <= 150) {
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
  } else {

    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
  }

  delay(15);
}
