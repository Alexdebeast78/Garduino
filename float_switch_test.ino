
const int floatswitch = 17;

void setup() {
  Serial.begin(115200);
  pinMode(floatswitch,INPUT_PULLUP);
}

void loop() {
  Serial.println(digitalRead(floatswitch));

}
