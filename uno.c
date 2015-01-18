void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  delay(10);
  digitalWrite(2, HIGH);
}

void loop() {
  delay(2000);
  Serial.println("Time is: " + String(millis()/1000));
}