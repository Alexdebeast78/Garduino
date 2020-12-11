//simple  reset
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C   u8g2(U8G2_R0, 16, 15, 4);

void setup() {
 
 
 u8g2.clearBuffer();
 Serial.begin(9600);
 Serial.print("Done");
}

void loop() {
  // put your main code here, to run repeatedly:
 Serial.println("DONE");
}
