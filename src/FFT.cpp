#include "arduinoFFT.h" // Standard Arduino FFT library https://github.com/kosme/arduinoFFT
#include "FFT.h"
#include "GoaStickLED.h"
arduinoFFT FFT = arduinoFFT();


#define SAMPLES 1024             // Must be a power of 2
#define SAMPLING_FREQUENCY 40000 // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define amplitude 150            // Depending on your audio source level, you may need to increase this value
unsigned int sampling_period_us;
void displayBand(int band, int dsize);
unsigned long microseconds;
byte peak[] = {0,0,0,0,0,0,0,0};
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime, oldTime;
int dominant_value;
/////////////////////////////////////////////////////////////////////////
void calc_sample_period() {
    sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
}

void fft_sample() {
  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros();
  //vReal[i] = analogRead(A0); // Using Arduino ADC nomenclature. A conversion takes about 1uS on an ESP32
    vReal[i] = analogRead(MIC_PIN);
  //vReal[i] = analogRead(VP); // Using logical name fo ADC port
  //vReal[i] = analogRead(36); // Using pin number for ADC port
    vImag[i] = 0;
    while ((micros() - newTime) < sampling_period_us) { /* do nothing to wait */ }
  }
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  Serial.println("-------------------------------------------------");
  for (int i = 2; i < (SAMPLES/2); i++){ // Don't use sample 0 and only the first SAMPLES/2 are usable.
    // Each array element represents a frequency and its value, is the amplitude. Note the frequencies are not discrete.
    if (vReal[i] > 1500) { // Add a crude noise filter, 10 x amplitude or more

      int mapping = map(i,7,80,0,NUM_LEDS);
      strip.setPixelColor(mapping,0x006600);

      if (i<=2 )             displayBand(125,(int)vReal[i]); // 125Hz
      if (i >2   && i<=4 )   displayBand(250,(int)vReal[i]); // 250Hz
      if (i >4   && i<=7 )   displayBand(500,(int)vReal[i]); // 500Hz
      if (i >7   && i<=15 )  displayBand(1000,(int)vReal[i]); // 1000Hz
      if (i >15  && i<=40 )  displayBand(2000,(int)vReal[i]); // 2000Hz
      if (i >40  && i<=70 )  displayBand(4000,(int)vReal[i]); // 4000Hz
      if (i >70  && i<=288 ) displayBand(8000,(int)vReal[i]); // 8000Hz
      if (i >288           ) displayBand(16000,(int)vReal[i]); // 16000Hz
      //Serial.println(i);
    }
  }
  Serial.println("-------------------------------------------------");
}


//change this function tp do something
void displayBand(int band, int dsize){
 

  Serial.print(band);
  Serial.print("Hz: ");
  Serial.println(dsize);
}

