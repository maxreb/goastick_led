#include <stdint.h>
#include "GoaStickLED.h"

void lauflicht(int spied)
{ //50

    uint8_t a, rueck, b = strip.numPixels();
    rueck = b - 6;
    for (a = 0; a < b - 6; a++)
    { //erster durchgang
        if (digitalRead(BUTTON_MODE_PIN))
            break;
        strip.setPixelColor(a - 1, 0, 0, 0);
        strip.setPixelColor(a, (10), 0, (250));
        strip.setPixelColor(a + 1, (50), 0, (210));
        strip.setPixelColor(a + 2, (90), 0, (170));
        strip.setPixelColor(a + 3, (130), 0, (130));
        strip.setPixelColor(a + 4, (170), 0, (90));
        strip.setPixelColor(a + 5, (210), 0, (90));
        strip.setPixelColor(a + 6, (250), 0, (50));
        strip.setPixelColor(a + 7, 0, 0, 0);

        strip.setPixelColor(rueck - 1, 0, 0, 0); //licht startet hinten
        strip.setPixelColor(rueck, 0, 10, 250);
        strip.setPixelColor(rueck + 1, 0, 50, 210);
        strip.setPixelColor(rueck + 2, 0, 90, 170);
        strip.setPixelColor(rueck + 3, 0, 130, 130);
        strip.setPixelColor(rueck + 4, 0, 170, 90);
        strip.setPixelColor(rueck + 5, 0, 210, 90);
        strip.setPixelColor(rueck + 6, 0, 250, 50);
        strip.setPixelColor(rueck + 7, 0, 0, 0);
        rueck--;
        strip.show();
        delay(spied);
    }
    rueck = 0;
    for (a = b - 6; a > 0; a--)
    { //zweiter durchgang
        if (digitalRead(BUTTON_MODE_PIN))
            break;
        strip.setPixelColor(a - 1, 0, 0, 0);
        strip.setPixelColor(a, 10, 0, 250);
        strip.setPixelColor(a + 1, 50, 0, 210);
        strip.setPixelColor(a + 2, 90, 0, 170);
        strip.setPixelColor(a + 3, 130, 0, 130);
        strip.setPixelColor(a + 4, 170, 0, 90);
        strip.setPixelColor(a + 5, 210, 0, 90);
        strip.setPixelColor(a + 6, 250, 0, 50);
        strip.setPixelColor(a + 7, 0, 0, 0);

        strip.setPixelColor(rueck - 1, 0, 0, 0); //licht startet hinten
        strip.setPixelColor(rueck, 0, 10, 250);
        strip.setPixelColor(rueck + 1, 0, 50, 210);
        strip.setPixelColor(rueck + 2, 0, 90, 170);
        strip.setPixelColor(rueck + 3, 0, 130, 130);
        strip.setPixelColor(rueck + 4, 0, 170, 90);
        strip.setPixelColor(rueck + 5, 0, 210, 90);
        strip.setPixelColor(rueck + 6, 0, 250, 50);
        strip.setPixelColor(rueck + 7, 0, 0, 0);
        rueck++;
        strip.show();
        delay(spied);
    }
}

void rainbowCycle(int spied)
{
    uint16_t i,j;
    for (j = 0; j < 256 * 5; j++)
    { // 5 cycles of all colors on wheel
        for (i = 0; i < strip.numPixels(); i++)
        {
            if (digitalRead(BUTTON_MODE_PIN))
                return;

            strip.setPixelColor(i, (Wheel(((i * 256 / strip.numPixels()) + j) & 255)));
        }

        strip.show();
        delay(spied);
    }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(int spied)
{
    static int j = 0;
    j++;
    if (j > 255)
        j = 0;

    for (int q = 0; q < 3; q++)
    {

        for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
        {
            strip.setPixelColor(i + q, Wheel((i + j) % 255)); //turn every third pixel on
        }
        strip.show();

        delay(spied);

        for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
        {
            strip.setPixelColor(i + q, 0); //turn every third pixel off
        }
    }
}