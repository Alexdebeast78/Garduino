const byte photoresistor = A7;

void setup() {
  Serial.begin(115200);
}

void loop() { 
  int light_value=analogRead(photoresistor);
  Serial.println(light_value);
  delay(300);
}
