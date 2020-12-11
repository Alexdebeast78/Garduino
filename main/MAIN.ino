#include <TimeLib.h>


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
#include "DHT.h" //DHT sensor 
#include "U8g2lib.h" //OLED screen
#include "hel tec.h"
 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //                                                  
// Constants.                                                                                              //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const byte DHTpin = 5;
const byte lightPin = 17;
const byte moisturePin = 18;Heltec ESP32
const byte DHTtype = DHT22;
const byte relay1PIN= 19;
const byte relay2PIN=23; 
const int dry = 595; 
const int wet = 239; 
const int smallFont = 8;
const int bigFont = 20; 
const int FONT_ONE_HEIGHT            =8;                    
const int FONT_TWO_HEIGHT              = 20;
const int floatswitch = 17;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //                                                  
// Variables.                                                                                              //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

U8G2_SSD1306_128X64_NONAME_F_HW_I2C   u8g2(U8G2_R0, 16, 15, 4);
int lightReading;
int moistureVal;
int percentHumidity;
int water;
float hum; 
float temp; 
char chBuffer[128];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //                                                  
// Welcome Screen.                                                                                         //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void drawCircle(void) {
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
}

void sayHello(void) {
  
  u8g2.setFont(u8g2_font_crox5tb_tr);
  u8g2.clearBuffer();
  sprintf(chBuffer, "%s", "Welcome To");
  u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 0, chBuffer);
  sprintf(chBuffer, "%s", "Garduino");
  u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 31 - (FONT_ONE_HEIGHT / 2), chBuffer);
  u8g2.sendBuffer();
  
    
}

void myName(void) {
  u8g2.setFont(u8g2_font_helvR12_tr);
  sprintf(chBuffer, "%s", "By Alex");
  u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2),52 - (FONT_ONE_HEIGHT / 2),chBuffer);
  u8g2.sendBuffer();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //                                                  
// Setup.                                                                                                  //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  // Serial setup
  
  Serial.begin(9600);
  while(!Serial)
  {
    Serial.print('.');
  }
  //Initialize sensors
  pinMode(floatSwitch,INPUT_PULLUP);
  DHT dht(DHTpin, DHTtype);
  dht.begin();
  pinMode(relay1PIN, OUTPUT);
  pinMode(relay2PIN, OUTPUT);
  //Initialize OLED graphics 
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
  Heltec.display->setContrast(255);
  Heltec.display->clear();
  drawCircle();
  delay(1000);
  Heltec.display->clear();
  u8g2.begin();
  u8g2.setFont(u8g2_font_crox5tb_tr);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
  sayHello();
  myName();

  

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //
// Sensors check.                                                                                          //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void sensorsCheck(void){
  // DHT sensor read data
  hum = dht.readHumidity(); 
  temp = dht.readTemperature();
  //Light sensor read data
  lightReading = analogRead(lightPin);
  //Moisture sensor read data
  moistureVal = analogRead(moisturePin);
  percentHum = map(moistureVal, wet, dry, 100, 0);
  //Water Level sensor read data
  water = digitalRead(floatswitch);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //
// Data to OLED.                                                                                           //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void display(void){
  sensorsCheck();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tr);
  if (isnan(hum) || isnan(temp)) { 
    sprintf(chBuffer, "%s", "No reading from");
    u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 0, chBuffer);
    sprintf(chBuffer, "%s", "DHT sensor");
    u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 13 - (FONT_ONE_HEIGHT / 2), chBuffer);
    u8g2.sendBuffer(); 
  }else{
    sprintf(chBuffer,"%s%f%s", "Temp: ",temp,"Celsius");
    u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 0 - (FONT_ONE_HEIGHT / 2), chBuffer);
    sprintf(chBuffer, "%s%f%s","Hum: ", hum, "%"
    u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 13 - (FONT_ONE_HEIGHT / 2), chBuffer);
    u8g2.sendBuffer();    
  }
  if (lightReading<100){
    sprintf(chBuffer,"%s","Leds are on baby")
    u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 26 - (FONT_ONE_HEIGHT / 2), chBuffer);
    u8g2.sendBuffer();
  }else{
    sprintf(chBuffer,"%s", "Leds: OFF"
    u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 26 - (FONT_ONE_HEIGHT / 2), chBuffer);
    u8g2.sendBuffer();
  }
  sprintf(chBuffer, "%s%is%","Soil moisture: ",percentHum,"%")
  u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 39 - (FONT_ONE_HEIGHT / 2), chBuffer);
  
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //
// Control Relays.                                                                                         //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void relays(void){
  sensorsCheck();
  if (lightReading<100){
    digitalWrite(relay1PIN, LOW);
  }else{
    digitalWrite(relay1PIN, HIGH);
  }
  if percentHum<20{
    digitalWrite(relay2PIN, LOW);
  }else if percentHum>60{
    digitalWrite(relay2PIN, HIGH);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //
// Main loop.                                                                                              //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  delay(2000);
  display();
  relays();
}
  
