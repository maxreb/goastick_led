#ifndef GOASTICK_h
#define GOASTICK_h

#include <inttypes.h>



#define NUM_LEDS 26 // total number of all connected LEDs, should be even
#define NUM_STRIPES 2 // number of stripes. On NUM_LEDS / NUM_STRIPES will be seperated

#define LED_PIN 13
#define MIC_PIN 33

#define BUTTON_MODE_PIN 34

#define MAX_LOUDNESS_SCALE 255


#define MAX_BRIGTHNESS 100//Maximale helligkeit, sollte auf 255 am ende sein

#define SCROLLVIS_FADEOUT 0.55//fuer den dritten Modus. Je kleiner die Zahl ist, desto weniger hell der effekt. Immer < 1.0!
#define SCROLLVIS_MINIMUM_CUT 0.15 //Bei wieviel prozent er unten rum cuttet (noise filter)


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