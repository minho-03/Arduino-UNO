int fan = 9; 
int analogPin = A3;   
int val = 0;         

void setup() {
  Serial.begin(9600);
  pinMode(fan, OUTPUT); 
}

void loop() {
  val = analogRead(analogPin);
  Serial.println(val); 
  analogWrite(fan, val / 4); 
}