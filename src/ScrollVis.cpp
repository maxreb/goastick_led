#include "GoaStickLED.h"

#define SCROLLVIS_FADEOUT 0.55
#define SCROLLVIS_MINIMUM_CUT 0.15


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

