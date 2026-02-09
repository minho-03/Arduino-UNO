const int ledPin = 13;
const int buttonPin = 2;

int buttonState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  
  if(buttonState == HIGH){
    Serial.println("LED ON");
      digitalWrite(ledPin, HIGH); 
      delay(1000);
  } else {
      digitalWrite(ledPin, LOW);
      delay(1000);
  }
}
