const int ledPin = 13;
const int ledPin1 = 12;
const int buttonPin = 2;

int buttonState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  
  if(buttonState == HIGH){
    Serial.println("LED ON");
      digitalWrite(ledPin, HIGH); 
      digitalWrite(ledPin1, HIGH); 
      delay(500);
  } else {
      digitalWrite(ledPin, LOW);
      digitalWrite(ledPin1, LOW);
      delay(500);
  }
}
