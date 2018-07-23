#ifndef GOASTICK_h
#define GOASTICK_h

#include <inttypes.h>



#define NUM_LEDS 16 // total number of all connected LEDs
#define NUM_STRIPES 2 // number of stripes. On NUM_LEDS / NUM_STRIPES will be seperated

#define LED_PIN 13
#define MIC_PIN 33

#define BUTTON_MODE_PIN 34

#define MAX_LOUDNESS_SCALE 100



#ifndef TEST
#if MAX_LOUDNESS_SCALE > 255
#error "as Led Colors is uint8_t scale is not allowed to be above 255"
#endif
extern uint32_t LedColors[NUM_LEDS];//TODO make it extern
extern int NumLEDsPerStripe;
uint32_t Wheel(uint8_t  WheelPos);
#include "Adafruit_NeoPixel.h"
extern Adafruit_NeoPixel strip;
#endif


#endif