#include "Arduino.h"

#define NUM_LEDS 15

byte LEDcalcMini[NUM_LEDS];
byte LEDcalcMaxi[NUM_LEDS];
byte LEDcalcNow[NUM_LEDS];
float LEDcalcDelta[NUM_LEDS];

unsigned long previousMillis = 0;
unsigned long interval = 1;                    //Wartezeit in ms

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 13
#define MIC_PIN 33

const int maxScale = 10;
const int sampling_rate_Hz = 20;
const int digital_ampl_min = 10;
const int digital_ampl_max = 250;
const int digital_ampl_cut = 7;

const int LEDparamDeltaClearance = 1;
const int LEDparamBrigthMax = 40;
const int LEDparamBrigthMin = 1;
const int LEDparamBrigthClearance = 5;

const int sampleWindow = 1000 / sampling_rate_Hz; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and LEDcalcMinimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  
  strip.begin();
  for (int i = 0; i < NUM_LEDS; i++) {
    SetPixel(i);
    strip.show();
    delay(33);
  }
  delay(1000);
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, 0);
    strip.show();
    delay(33);
  }

  randomSeed(analogRead(MIC_PIN));

  /*delay(500);
    for (uint16_t i = 0; i < 5; i++) {
    strip.setPixelColor(i, 0xffffff);
    strip.show();
    delay(50);
    }
    //delay(10000);*/
  SetParameters();
  
}

void loop() {
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  static int lastPP = 100;

  while (millis() - startMillis < sampleWindow) {

    sample = analogRead(MIC_PIN);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax) {
        signalMax = sample;  // save just the max levels
      } else if (sample < signalMin) {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;
  if (lastPP < digital_ampl_cut) {
    lastPP = peakToPeak;
    peakToPeak = 0;
  } else
    lastPP = peakToPeak;

  //Serial.println(peakToPeak);

  // map 1v p-p level to the max scale of the display
  int displayPeak = map(peakToPeak, digital_ampl_min, digital_ampl_max, 0, maxScale);

  CalculateValues(displayPeak);
  delay(1);//this is important to avoid flickering of the LEDs

  strip.show();
}

void SetParameters() {
  for (int i = 0; i < NUM_LEDS; i++) {        // alle LEDs
    LEDcalcMini[i] = random(LEDparamBrigthMin, LEDparamBrigthMin + LEDparamBrigthClearance); // LEDcalcMinimale Helligkeit (0-50)
    LEDcalcMaxi[i] = random(LEDparamBrigthMax - LEDparamBrigthClearance, LEDparamBrigthMax); // LEDcalcMaximale Helligkeit (100-255)
    LEDcalcDelta[i] = random(LEDparamDeltaClearance * -100, LEDparamDeltaClearance * 100) / 100.0; // Schrittweite zwischen -5 und +5

    LEDcalcNow[i] = random(LEDparamBrigthMin, LEDparamBrigthMax);       // Startwerte
  }
}
void CalculateValues(int addPeak) {                            // jede LED hoch- oder runterz�hlen
  unsigned long currentMillis = millis();
  int test = 1;
  for (int i = 0; i < NUM_LEDS; i++) {
  if (i < addPeak) {
   SetPixel(i);
    } else {
      strip.setPixelColor(i, 0x000000);
    }
  }




  /*
  	if (currentMillis - previousMillis > interval) { // Wartezeit
  		previousMillis = currentMillis;

  		for (int i = 0; i < NUM_LEDS; i++) {   //

  			if (LEDcalcNow[i] + LEDcalcDelta[i] > LEDcalcMaxi[i]) {   // w�re der n�chste Schritt au�erhalb LEDcalcMaxi?
  				LEDcalcDelta[i] = random(-1, LEDparamDeltaClearance * -1);    // neu LEDcalcDelta
  				LEDcalcNow[i] = LEDcalcNow[i] + LEDcalcDelta[i];     // weiterz�hlen
  				/*Serial.print("LEDcalcNow");
  				 Serial.print(LEDcalcNow[i]);
  				 Serial.print("LEDcalcDelta");
  				 Serial.println(LEDcalcDelta[i]);
  			}

  			else if (LEDcalcNow[i] + LEDcalcDelta[i] < LEDcalcMini[i]) { // w�re der n�chste Schritt au�erhalb LEDcalcMini?
  				LEDcalcMaxi[i] = random(LEDparamBrigthMax - LEDparamBrigthClearance, LEDparamBrigthMax); // neu LEDcalcMaxi
  				LEDcalcDelta[i] = random(1, LEDparamDeltaClearance);            // neu LEDcalcDelta
  				LEDcalcNow[i] = LEDcalcNow[i] + LEDcalcDelta[i];         // weiterz�hlen

  				/*  Serial.print("LEDcalcNow");
  				 Serial.print(LEDcalcNow[i]);
  				 Serial.print("LEDcalcDelta");
  				 Serial.println(LEDcalcDelta[i]);
  			} else {                              // wenn n�chster Schritt innerhalb LEDcalcMini und LEDcalcMaxi
  				if (LEDcalcDelta[i] < 0)
  					LEDcalcNow[i] = LEDcalcNow[i] + LEDcalcDelta[i] - addPeak;      // sofort weiterz�hlen
  				else
  					LEDcalcNow[i] = LEDcalcNow[i] + LEDcalcDelta[i] + addPeak;      // sofort weiterz�hlen

  			}
  			//Serial.print(LEDcalcNow[i],DEC);
  			//Serial.print(" vs ");

  			//Serial.print(i,DEC);
  			//Serial.print(" --> ");
  			//Serial.println((i+2)%3);
  			test = LEDcalcNow[i];

  			switch (addPeak) {
  				case 0 ... maxScale * 1 / 3:
  					if (!((i) % 3)) {
  						test = LEDcalcNow[i] + addPeak / 2;
  						//Serial.print(i,DEC);
  						//Serial.println(" --> 1");
  					}
  					break;
  				case (maxScale * 1 / 3) + 1 ... (maxScale * 2 / 3):
  					if (!((i + 1) % 3)) {
  						test = LEDcalcNow[i] + map(addPeak, maxScale * 1 / 3, maxScale * 2 / 3, 0, maxScale);
  						//Serial.print(i,DEC);
  						//Serial.println(" --> 2");
  					}
  					break;
  				case (maxScale * 2 / 3) + 1 ... maxScale:
  					if (!((i + 2) % 3)) {
  						test = LEDcalcNow[i] + map(addPeak, maxScale * 2 / 3, maxScale, 0, maxScale * 2);
  						//Serial.print(i,DEC);
  						//Serial.println(" --> 3");
  					}
  					break;
  			}

  			test = min(0xff, test);            //nicht gr��er als max value
  			//Serial.println(LEDcalcNow[i],DEC);

  			strip.setPixelColor(i, strip.Color(test, test, test));
  		}

  	}*/
}


void SetPixel(int i) {
  if (i < NUM_LEDS / 2)
    strip.setPixelColor(i, 0x005500);
  else if ( i >= NUM_LEDS / 2 && i < (NUM_LEDS / 3) * 2)
    strip.setPixelColor(i, 0x555500);
  else
    strip.setPixelColor(i, 0x550000);
}
