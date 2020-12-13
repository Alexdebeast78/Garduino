///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //                                                  
// Garduino with Heltec WiFi Kit 32                                                                        //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//There will be some notes here about the project
//
//
//
//
//
//
//
//
//
//
//int floatswitch = 7;

//

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //                                                  
// Includes.                                                                                               //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <heltec.h>
#include <Arduino.h>
#include <U8g2lib.h> 
#include <DHT.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //                                                  
// Constants.                                                                                              //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const byte DHTPIN_22 = 14;
const byte DHTPIN_11 = 27;

const int FONT_ONE_HEIGHT = 8;
const int FONT_TWO_HEIGHT = 20;

char      chBuffer[128];


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //                                                  
// Variables.                                                                                              //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DHT dht11(DHTPIN_11, DHT11);
DHT dht22(DHTPIN_22, DHT22);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C   u8g2(U8G2_R0, 16, 15, 4); 



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //                                                  
// Setup.                                                                                                  //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200); // send and receive at 9600 baud
  delay(1000);
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
  Heltec.display->setContrast(255);
  Heltec.display->clear();

  //Welcome screen 
  Serial.println("WELCOME SCREEN");
  for (int16_t i=0; i<DISPLAY_HEIGHT; i+=2) {
    Heltec.display->drawCircle(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, i);
    Heltec.display->display();
    delay(10);
  }
  delay(1000);
  Heltec.display->clear();
  // This will draw the part of the circel in quadrant 1
  // Quadrants are numberd like this:
  //   0010 | 0001
  //  ------|-----
  //   0100 | 1000
  //
  Heltec.display->drawCircleQuads(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, DISPLAY_HEIGHT/4, 0b00000001);
  Heltec.display->display();
  delay(200);
  Heltec.display->drawCircleQuads(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, DISPLAY_HEIGHT/4, 0b00000011);
  Heltec.display->display();
  delay(200);
  Heltec.display->drawCircleQuads(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, DISPLAY_HEIGHT/4, 0b00000111);
  Heltec.display->display();
  delay(200);
  Heltec.display->drawCircleQuads(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, DISPLAY_HEIGHT/4, 0b00001111);
  Heltec.display->display();
  //Small Circle 
  delay(1000);
  Heltec.display->clear();
  u8g2.begin();
  u8g2.setFont(u8g2_font_crox5tb_tr);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
  //My name 
  u8g2.clearBuffer();
  sprintf(chBuffer, "%s", "Welcome to");
  u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 0, chBuffer);
  sprintf(chBuffer, "%s", "Garduino");
  u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 31 - (FONT_ONE_HEIGHT / 2), chBuffer);
  u8g2.sendBuffer();
  u8g2.setFont(u8g2_font_helvR12_tr);
  sprintf(chBuffer, "%s", "by Alexdebeast78");
  u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2),52- (FONT_ONE_HEIGHT / 2),chBuffer);
  u8g2.sendBuffer();
  delay(5000);
  u8g2.clear();

  //DHT sensors  begin 
  dht11.begin();
  dht22.begin();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //
// Main loop.                                                                                              //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
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


