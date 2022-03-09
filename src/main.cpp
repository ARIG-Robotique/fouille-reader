//#define DEBUG
//#define SCREEN

#include <Arduino.h>
#if defined(SCREEN)
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif
#include <FastLED.h>
#include <Wire.h>

#define NUM_LEDS 1

#if defined(SCREEN)
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

void printCarreFouille(int value, String name) {
#if defined(SCREEN)
  display.clearDisplay();

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(name);
  display.println(value, DEC);

  display.display();
#endif
}

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Setup");
#endif

  analogReference(EXTERNAL);
  pinMode(A0, INPUT);

#if defined(SCREEN)
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
  #if defined(DEBUG)
    Serial.println(F("SSD1306 allocation failed"));
  #endif
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
#endif

}

boolean between(int value, int medium) {
  return (value >= medium - 10 && value <= medium + 10);
}

int lastValue = 0;
int value = 0;
int cpt = 0;

void loop() {
  value = analogRead(A0);
  int diff = abs(value - lastValue);
  Serial.print("Value : "); Serial.print(value, DEC); 
  Serial.print("\tDiff  : "); Serial.println(diff, DEC);

  if (diff < 5) {
    cpt++;
  } else {
    cpt = 0;
  }
  lastValue = value;

  if (cpt > 3) {
    cpt = 3;
    if (between(value, 327)) {
      printCarreFouille(value, "VIOLET");
    } else if (between(value, 511)) {
      printCarreFouille(value, "JAUNE");
    } else if (between(value, 843)) {
      printCarreFouille(value, "INTERDIT");
    } else {
      printCarreFouille(value, "INCONNU");
    }
  } else {
    printCarreFouille(value, "EN L'AIR");
  }

  delay(2);
}
