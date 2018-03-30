/*****************************************************
 * ESP32 DHT Reading and OLED diplay 
 * DHT Input: ==> GPIO22.
 * 128 X 32 SSD1306  - Library by Daniel Eichhorn
 * 
 * MJRoBot.org 12Sept17
 *****************************************************/
 
/* modified font created at http://oleddisplay.squix.ch/ */
#include "modified_font.h"

/*  SSD1306 */
#include "SSD1306.h" 
#define SDA_PIN 4// GPIO21 -> SDA
#define SCL_PIN 15// GPIO22 -> SCL
#define SSD_ADDRESS 0x3c
SSD1306  display(SSD_ADDRESS, SDA_PIN, SCL_PIN);

/* DHT */
#include "DHT.h"
#define DHTPIN 22  
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
float localHum = 0;
float localTemp = 0;

void setup() {

  pinMode(16,OUTPUT);
  pinMode(2,OUTPUT);
  
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in hi
  Serial.begin(115200);
  Serial.println("");
  Serial.println("ESP32 DHT Temperature and Humidity - OLED Display");
  Serial.println("");

  dht.begin();

  display.init();

  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(Open_Sans_Condensed_Light_20); // set a font
}

void loop() 
{
  getDHT();
  displayData();
  delay(2000);
}

/***************************************************
* Get indoor Temp/Hum data
****************************************************/
void getDHT()
{
  float tempIni = localTemp;
  float humIni = localHum;
  localTemp = dht.readTemperature();
  localHum = dht.readHumidity();
  if (isnan(localHum) || isnan(localTemp))   // Check if any reads failed and exit early (to try again).
  {
    localTemp = tempIni;
    localHum = humIni;
    return;
  }
}

/***************************************************
* Display Data
****************************************************/
void displayData() 
{
  Serial.print("Temp: ==> ");
  Serial.print(localTemp);
  Serial.print("  Hum ==> ");
  Serial.println(localHum);

  display.clear();   // clear the display

  display.drawString(0, 0,  "temp: ");
  display.drawString(40, 0,  String(localTemp));
  display.drawString(90, 0,  "oC");
  display.drawString(0, 32, "hum:  ");
  display.drawString(40, 32,  String(localHum));
  display.drawString(90, 32,  "%");

  display.display();   // write the buffer to the display
  delay(10);
}


