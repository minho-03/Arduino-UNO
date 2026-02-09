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
  Serial.println(buttonState);
  if(buttonState == LOW){
      digitalWrite(ledPin, HIGH); 
  } else {
      digitalWrite(ledPin, LOW);
  }
}
