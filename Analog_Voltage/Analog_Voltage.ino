int ledPin = 9;
int analogPin = A3;
int val = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.0);
  val = analogRead(analogPin);
  analogWrite(ledPin, val / 4);
  Serial.println(voltage);
}
