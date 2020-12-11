#include <heltec.h>
#include "Arduino.h"
#include <U8g2lib.h>  
char      chBuffer[128];
#define   FONT_ONE_HEIGHT               8                                   // font one height in pixels
#define   FONT_TWO_HEIGHT               20 


U8G2_SSD1306_128X64_NONAME_F_HW_I2C   u8g2(U8G2_R0, 16, 15, 4); 

void setup() {
  Serial.begin(115200); // send and receive at 9600 baud
  
 
  
 
  

  

 

}

void drawCircle(void) {
  Serial.println("why cant you just print");
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
  sprintf(chBuffer, "%s", "!ALERT!");
  u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 0, chBuffer);
  sprintf(chBuffer, "%s", "!FRAUD!");
  u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 31 - (FONT_ONE_HEIGHT / 2), chBuffer);
  u8g2.sendBuffer();
  
    
}

void myName(void) {
  u8g2.setFont(u8g2_font_helvR12_tr);
  sprintf(chBuffer, "%s", "Voting Machine");
  u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2),52- (FONT_ONE_HEIGHT / 2),chBuffer);
  u8g2.sendBuffer();
}











void loop(void) {
  delay(1000);
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
  u8g2.clearBuffer();
  
}
