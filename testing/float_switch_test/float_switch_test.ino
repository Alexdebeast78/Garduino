const byte floatswitch = 17;

void setup() {
  Serial.begin(115200);
  pinMode(floatswitch,INPUT_PULLUP);
}

void loop() {
  int water = digitalRead(floatswitch);
  if (water==1){
    Serial.println("Water is empty!");
  }
}
