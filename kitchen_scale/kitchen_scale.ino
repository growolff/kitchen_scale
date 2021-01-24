
#include "HX711.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BLACK SSD1306_BLACK
#define WHITE SSD1306_WHITE

HX711 scale;

#define DOUT 5
#define CLK  6

float calibration_factor = 940; // this factor can change depending on your gauge configuraton
float weight;

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  Serial.println(F("SSD1306 display begin :)"));

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,10);
  display.println("masa[g]: ");
  display.display();
  delay(500);
    
  scale.begin(DOUT, CLK);

  scale.set_scale();  // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();            // reset the scale to 0

  scale.set_scale(calibration_factor); //Adjust to this calibration factor
}

void loop() {
  weight = scale.get_units(10);

  display.clearDisplay();
  display.setCursor(10,10);
  display.println("masa[g]: ");
  display.setCursor(60,10);
  display.println(weight); 
  display.display();
  
  Serial.print("weigh: ");
  Serial.print(weight, 2);
  Serial.println();

  delay(50);
}
