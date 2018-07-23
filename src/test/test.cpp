//#define TEST
#ifdef TEST
#include "..\GoaStickLED.h"
#include <cstdio>
#include <inttypes.h>


uint32_t LedColors[NUM_LEDS];

int NumLEDsPerStripe = NUM_LEDS / NUM_STRIPES;
 


void ScrollVis(int loudness);
void Display();

int main(){
	int n = 0;
	
	for (int i = 0; i< NUM_LEDS/NUM_STRIPES;i++){
		printf("%u/\t",i);
	}
	printf("\n");
	
	//return 0;
	while (n++ < 20){
		if (n == 1 || n== 2 || n == 10)
		ScrollVis(75);
	else
		ScrollVis(9);
		Display();
	}
}
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void ScrollVis(int loudness)
{
 	static int centerPerStripe = (NumLEDsPerStripe / 2);
    //int pixl = map(loudness,0,MAX_LOUDNESS_SCALE,0,NUM_LEDS / NUM_STRIPES);

    for (int i = 0; i < NUM_STRIPES; i++)
    {
        int start = i * NumLEDsPerStripe;
        int center = start + centerPerStripe;
        for (int n = start; n < start + NumLEDsPerStripe; n++)
        {
        	
            if (LedColors[n] > 0)
            {

                if (n < center && n > start)
                {
                    LedColors[n - 1] = LedColors[n] / 1.5;
                }
                else if (n > center && n < start + NumLEDsPerStripe - 1)
                {
                    LedColors[n + 1] = LedColors[n] / 1.5;
                }
            }
        }
        for (int n = start; n < start + NumLEDsPerStripe; n++)
        {
        	
			LedColors[n] *= SCROLLVIS_FADEOUT;

        }

        if (loudness > MAX_LOUDNESS_SCALE * SCROLLVIS_MINIMUM_CUT){
        	//printf(".%u - %u - %u.\n",center,start, centerPerStripe);
            LedColors[center] += loudness;
	        if (LedColors[center] > MAX_LOUDNESS_SCALE)
	            LedColors[center] = MAX_LOUDNESS_SCALE;
	        LedColors[center + 1] = LedColors[center] / 2;
	        LedColors[center - 1] = LedColors[center + 1];
        }
    }
}




void Display(){
	for (int i = 0; i< NUM_LEDS/NUM_STRIPES;i++){
		printf("%u\t",LedColors[i]);
	}
	printf("\n");
}
#endif