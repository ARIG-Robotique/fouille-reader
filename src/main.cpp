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

enum CarreFouille : byte {
  EN_L_AIR,
  INCONNU,
  JAUNE,
  VIOLET,
  INTERDIT
};

// Private variables //
// ----------------- //
int i2cAddress = 0x3C;

int lastAnalogValue = 0;
int analogValue = 0;
int cpt = 0;

volatile CarreFouille carreFouille = EN_L_AIR;

CRGB leds[NUM_LEDS];

// Prototypes for functions defined at the end of this file //
// -------------------------------------------------------- //
#if defined(DEBUG)
void printCarreFouilleDebug(CarreFouille carreFouille);
#endif
#if defined(SCREEN) 
void printCarreFouilleScreen(int value, String name);
#endif
void i2cRequest();
boolean between(int value, int medium);

// Configuration //
// ------------- //
void setup()
{
#if defined(DEBUG)
  Serial.begin(115200);
  Serial.println("Setup");

  Serial.println(" - Configuration des I/O");
#endif

  analogReference(EXTERNAL);
  pinMode(A0, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

#if defined(SCREEN)
  #if defined(DEBUG)
    Serial.println(" - Configuration Ecran OLED");
  #endif
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

  
  Wire.begin(i2cAddress);
  Wire.onRequest(i2cRequest);
#if defined(DEBUG)
  Serial.print(" - I2C [OK] (Addresse : ");
  Serial.print(i2cAddress, HEX);
  Serial.println(")");
#endif

}

// Main loop //
// --------- //
void loop() {
  analogValue = analogRead(A0);
  int diff = abs(analogValue - lastAnalogValue);

  if (diff < 5) {
    cpt++;
  } else {
    cpt = 0;
  }
  lastAnalogValue = analogValue;

  if (cpt > 3) {
    cpt = 3;
    if (between(analogValue, 327)) {
#if defined(SCREEN)      
      printCarreFouilleScreen(analogValue, "VIOLET");
#endif
      carreFouille = VIOLET;
      leds[0] = CRGB::Purple;
    
    } else if (between(analogValue, 511)) {
#if defined(SCREEN)       
      printCarreFouilleScreen(analogValue, "JAUNE");
#endif     
      carreFouille = JAUNE;
      leds[0] = CRGB::Yellow;
    
    } else if (between(analogValue, 843)) {
#if defined(SCREEN)     
      printCarreFouilleScreen(analogValue, "INTERDIT");
#endif
      carreFouille = INTERDIT;
      leds[0] = CRGB::Red;
    
    } else {
#if defined(SCREEN)       
      printCarreFouilleScreen(analogValue, "INCONNU");
#endif
      carreFouille = INCONNU;
      leds[0] = CRGB::Blue;
    }
  
  } else {
#if defined(SCREEN) 
    printCarreFouilleScreen(analogValue, "EN L'AIR");
#endif
    carreFouille = EN_L_AIR;
    leds[0] = CRGB::Blue;
  }

  FastLED.show();
  FastLED.delay(2);

  EVERY_N_SECONDS(1) {
    digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) == LOW ? HIGH : LOW);

#if defined(DEBUG)
    Serial.print("Value : "); Serial.print(analogValue, DEC); 
    Serial.print("\tDiff  : "); Serial.println(diff, DEC);
    printCarreFouilleDebug(carreFouille);
#endif
  }
}

void i2cRequest() {
  Wire.write(carreFouille);
}

#if defined(DEBUG)
void printCarreFouilleDebug(CarreFouille carreFouille) {
  Serial.print("Carre de fouille : 0x"); 
  Serial.println(carreFouille, HEX); 
}
#endif

#if defined(SCREEN)
void printCarreFouilleScreen(int value, String name) {
  display.clearDisplay();

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(name);
  display.println(value, DEC);

  display.display();
}
#endif

boolean between(int value, int medium) {
  return (value >= medium - 10 && value <= medium + 10);
}