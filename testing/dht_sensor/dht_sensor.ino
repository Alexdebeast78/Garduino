#include <DHT.h>

const byte DHTPIN_11 = 27;
const byte DHTPIN_22 = 14;


// Initialize DHT sensors for normal Arduino
DHT dht11(DHTPIN_11, DHT11);
DHT dht22(DHTPIN_22, DHT22);

void setup() {
  Serial.begin(115200); 
  Serial.println("DHTxx test!");
 
  dht11.begin();
  dht22.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h_11 = dht11.readHumidity();
  float h_22 = dht22.readHumidity();
  // Read temperature as Celsius
  float t_11 = dht11.readTemperature();
  float t_22 = dht22.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h_11) && isnan(t_11) && isnan(h_22) && isnan(t_22)) {
    Serial.println("Failed to read from DHT sensors!");
    return;
  }
  else if (isnan(h_11) || isnan(t_11)) {
    Serial.println("Failed to read from DHT11 sensor!");
    return;
  }
  else if (isnan(h_22) || isnan(t_22)) {
    Serial.println("Failed to read from DHT22 sensor!");
    return;
  }
  
  Serial.print("DHT11 Humidity: "); 
  Serial.print(h_11);
  Serial.print(" %\t");
  Serial.print("DHT11 Temperature: "); 
  Serial.print(t_11);
  Serial.println(" *C ");

  Serial.print("DHT22 Humidity: "); 
  Serial.print(h_22);
  Serial.print(" %\t");
  Serial.print("DHT22 Temperature: "); 
  Serial.print(t_22);
  Serial.println(" *C "); 
}
