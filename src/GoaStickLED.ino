#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
#include "FFT.h"
#include "GoaStickLED.h"
#include "ScrollVis.h"

#ifdef __AVR__
#include <avr/power.h>
#endif

//Scale analog ampl to 0 to MAX_LOUDNESS_SCALE
const int sampling_rate_Hz = 100;
const int digital_ampl_min = 10;                  //Starting the Analog Signal from this to...
const int digital_ampl_max = 250;                 //...this
const int digital_ampl_cut = 15;                  //everything under this will be cut (background noise)
const int sampleWindow = 1000 / sampling_rate_Hz; // Sample window width in mS (50 mS = 20Hz)

unsigned int sample;
u_char Mode = 0;

uint32_t LedColors[NUM_LEDS];
int NumLEDsPerStripe = NUM_LEDS / NUM_STRIPES;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
uint32_t SetPixel(int i);
void CalculateColoredLoudnessValues(int addPeak);

void Display()
{
  for (int i = 0; i < NUM_LEDS / NUM_STRIPES; i++)
  {
    Serial.print(LedColors[i]);
    Serial.print(", ");
  }
  Serial.println("");
}

void setup()
{
  Serial.begin(115200);
  strip.begin();
  for (int i = 0; i < NUM_LEDS; i++)
  {
    SetPixel(i);
    strip.show();
    delay(33);
  }
  delay(1000);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    strip.setPixelColor(i, 0);
    strip.show();
    delay(33);
  }

  pinMode(BUTTON_MODE_PIN, INPUT);
  pinMode(MIC_PIN, INPUT);

  randomSeed(analogRead(MIC_PIN));
  /*
  while (1){
      Serial.println(digitalRead(BUTTON_MODE_PIN));
      delay(100);
  }*/
}

void loop()
{
  static int buttonState = 0;

  if (buttonState == 0 && digitalRead(BUTTON_MODE_PIN) == 1)
  {
    Serial.print("mode++: ");
    Mode++;
    Serial.println(Mode);
  }

  buttonState = digitalRead(BUTTON_MODE_PIN);

  strip.clear();                           //reset strip
  memset(LedColors, 0, sizeof(LedColors)); //reset

  switch (Mode)
  {
  case 0:
    Mode_LoudnessColor();
    break;
  case 1:
    Mode_FFTDefault();
    break;
  case 2:
    Mode_LoudnessCenter();
    break;
  default:
    Serial.println(Mode);
    Mode = 0;
  }

  for (int i = 0; i < NUM_LEDS; i++)
    strip.setPixelColor(i, LedColors[i]);
  delay(10);
  /*

  CalculateColoredLoudnessValues
(displayPeak);
  */
  delay(1); //this is important to avoid flickering of the LEDs
  strip.show();
}

int calculateLoudness()
{
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;          // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  static int lastPP = 100;
  //Calculte peek to peek
  while (millis() - startMillis < sampleWindow)
  {

    sample = analogRead(MIC_PIN);
    if (sample < 1024) // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample; // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample; // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;
  //Cut all under digital_ampl_cut (Default 7)
  if (lastPP < digital_ampl_cut)
  {
    lastPP = peakToPeak;
    peakToPeak = 0;
  }
  else
    lastPP = peakToPeak;

  //Serial.println(peakToPeak);

  // map 1v p-p level to the max scale of the display
  return map(peakToPeak, digital_ampl_min, digital_ampl_max, 0, MAX_LOUDNESS_SCALE);
}

void Mode_FFTDefault()
{
  calc_sample_period();
  fft_sample();
}

void Mode_LoudnessColor()
{
  CalculateColoredLoudnessValues(calculateLoudness());
}

void Mode_LoudnessCenter()
{
  int able = calculateLoudness();
  ScrollVis(able);
  //Serial.print(able);
  //Serial.print(" -> ");
  //Display();
}

//Calcultate LED Values
void CalculateColoredLoudnessValues(int addPeak)
{
  int pixel = map(addPeak, 0, MAX_LOUDNESS_SCALE, 0, NumLEDsPerStripe);
  unsigned long currentMillis = millis();
  int stripe = 0;
  for (int i = 0; i < NUM_LEDS; i++)
  {
    int _i = i / NumLEDsPerStripe;
    _i = i - (_i * NumLEDsPerStripe);
    if (_i < pixel)
    {

      LedColors[i] = SetPixel(_i);
    }
    else
    {
      //strip.setPixelColor(i, 0x000000);
    }
  }
}

//Set pixel to the right color depending on which position it is
uint32_t SetPixel(int i)

{
  if (i < NUM_LEDS / 2)
    return strip.Color(0, MAX_LOUDNESS_SCALE, 0);
  else if (i >= NUM_LEDS / 2 && i < (NUM_LEDS / 3) * 2)
    return strip.Color(MAX_LOUDNESS_SCALE, MAX_LOUDNESS_SCALE, 0);
  else
    return strip.Color(MAX_LOUDNESS_SCALE, 0, 0);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(uint8_t WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}