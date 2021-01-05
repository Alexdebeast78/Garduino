const int dry=2900;
int moisture_level;
const byte pump_pin=18;
const byte moist_moist_pin=34;  
int x;
//Yeah fuck sorting out the variables 

void setup() {
  Serial.begin(115200);
  pinMode(pump_pin, OUTPUT);
}

void loop() {
  moisture_level=analogRead(moist_moist_pin);
  Serial.println(moisture_level);
  delay(100);
  if (moisture_level>dry){
      digitalWrite(pump_pin, HIGH);
      delay(2000);
      digitalWrite(pump_pin, LOW);
      //You could also use a  while loop depending on how much pressure your pump gives and your tubing(didn't really work for my setup)
      //I would advise changing the delay between pump and to find what suits your system best. 
      //while (x<=10){
      //  digitalWrite(pump_pin, HIGH);
      //  delay(200);
      //  digitalWrite(pump_pin, LOW);
      //  delay(200);
      //  x+=1
      //}

    
  x=0;
    
  }
  delay(5000);
}
