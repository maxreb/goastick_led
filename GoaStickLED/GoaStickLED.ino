#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif


#define NUM_LEDS 15
#define PIN 13
#define MIC_PIN 33

const int maxScale = 10; //Scale analog ampl to 0 to maxScale
const int sampling_rate_Hz = 20;
const int digital_ampl_min = 10;//Starting the Analog Signal from this to...
const int digital_ampl_max = 250;//...this 
const int digital_ampl_cut = 7;//everything under this will be cut (background noise)
const int sampleWindow = 1000 / sampling_rate_Hz; // Sample window width in mS (50 mS = 20Hz)

unsigned int sample;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
void SetPixel(int i);
void CalculateValues(int addPeak);


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
}

void loop() {
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  static int lastPP = 100;

  //Calculte peek to peek
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
  //Cut all under digital_ampl_cut (Default 7)
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

//Calcultate LED Values
void CalculateValues(int addPeak) {
  unsigned long currentMillis = millis();
  int test = 1;
  for (int i = 0; i < NUM_LEDS; i++) {
  if (i < addPeak) {
   SetPixel(i);
    } else {
      strip.setPixelColor(i, 0x000000);
    }
  }
}

//Set pixel to the right color depending on which position it is
void SetPixel(int i) {
  if (i < NUM_LEDS / 2)
    strip.setPixelColor(i, 0x005500);
  else if ( i >= NUM_LEDS / 2 && i < (NUM_LEDS / 3) * 2)
    strip.setPixelColor(i, 0x555500);
  else
    strip.setPixelColor(i, 0x550000);
}
