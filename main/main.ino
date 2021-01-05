///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //                                                  
// Garduino with Heltec WiFi Kit 32                                                                        //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //                                                  
// Includes.                                                                                               //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <heltec.h>
#include <Arduino.h>
#include <U8g2lib.h> 
#include <DHT.h>
#include <TimeLib.h>
#include <WiFi.h>
#include <WiFiUdp.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //                                                  
// Constants.                                                                                              //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
const byte DHTPIN_22 = 14;
const byte DHTPIN_11 = 27;
const byte floatswitch = 17;
const byte led_pin = 5;
const byte light_sensor = A7;

const int FONT_ONE_HEIGHT = 8;
const int FONT_TWO_HEIGHT = 20;
const int timeZone = 1;     // CET
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
const int dry=2900;

char      chBuffer[128];
const char ssid[] = "";  //  your network SSID (name)
const char pass[] = "";       // your network password
static const char ntpServerName[] = "0.dk.pool.ntp.org";

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //                                                  
// Variables.                                                                                              //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
DHT dht11(DHTPIN_11, DHT11);
DHT dht22(DHTPIN_22, DHT22);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C   u8g2(U8G2_R0, 16, 15, 4); 
WiFiUDP Udp;
time_t prevDisplay = 0; // when the digital clock was displayed

bool just_on;
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
unsigned int localPort = 8888;  // local port to listen for UDP packets

int moisture_level;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //                                                  
// Setup.                                                                                                  //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);

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

  //Connect to wifi 
  Serial.println("TimeNTP Example");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
  
  //Start UDP 
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");

  //Get time from NTP server 
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  Serial.println(hour());
  
  //DHT sensors  begin 
  dht11.begin();
  dht22.begin();

  //Float switch set pin type 
  pinMode(floatswitch,INPUT_PULLUP);

  //Led setup 
  pinMode(led_pin, OUTPUT);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //
// Main loop.                                                                                              //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
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

  //Float switch check for empty water 
  int water = digitalRead(floatswitch);
  if (water==1){
    Serial.println("Water is empty!");
  }
  
  //Get light  value
  int light_reading = analogRead(light_sensor);
  Serial.println(light_reading);

  //Get moisture level
  moisture_level=analogRead(34);
  Serial.println(moisture_level);  
  if (moisture_level>dry){
    Serial.println("Soil is dry!");
  }
  //Get time
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      //this could be useful for my oled
      prevDisplay = now();
      digitalClockDisplay();
    }
  }

  //Check time and light level
  if (minute() % 5 == 0 && not just_on) {
    if (hour() == 4 || hour() == 5 || hour() == 6 || hour() == 7 || hour() == 8 || hour() == 9 || hour() == 10 || hour() == 11 || hour() == 12 || hour() == 13 || hour() == 14 || hour() == 15 || hour() == 16 || hour() == 17 || hour() == 18 || hour() == 19 || hour() == 20) {
      if (light_reading < 100) {
        digitalWrite(led_pin, HIGH);
        Serial.println("LED strip is on!");
        just_on = true;

      }
      else {
        digitalWrite(led_pin, LOW);
        Serial.println("LED strip is off!");
      }
    }
    else {
      digitalWrite(led_pin, LOW);
      Serial.println("LED strip is off!");
    }
  }
    
  //Set just_on to false when conditional is not running 
  if (minute() % 5 == 0) {
  }
  else {
    just_on = false;
  }

  // Wait a few seconds between measurements.
  delay(5000);



}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //
// Display time.                                                                                           //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits)
{
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                          //
// NTP Code.                                                                                               //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address
  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
