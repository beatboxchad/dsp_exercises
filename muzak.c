/* 
 * this is written with some code a friend wrote for me and chapter 1 of
 * Richard Lyons' "Understanding Digital Signal Processing" open for reference.
 * It is the code of a newbie and should not be viewed as anything close to a
 * proper approach or best practice. I'm just writing it so that I understand
 * the concepts.
 * 
 * Thanks to my friend Brian Blackmore for writing the original Perl version of
 * this over coffee back in 2013, a couple years before I would have the time
 * and energy to dive into DSP in earnest. I asked him for a primer on audio
 * synthesis and he wrote the perl as fast as he could talk.
 * 
 * The sound does not encode right in this version. I pipe it to sox like so:
 * 
 * ./muzak | play -t raw -b 8 -e unsigned -c 2 -r 22000 - 
 *
 * ... and it sounds like crap, but it does play notes for the duration. I
 * haven't checked whether they're accurate in terms of absolute pitch, but it
 * definitely sounds very clippy and gross.
 *
 * The perfectionist in me wants to fix that before sharing this, but the fact
 * is that in the process of typing out this code I've learned how to think of
 * the frequency and time domains and how to apply difference equations to a
 * list of samples, so I should probably just move forward.
 *
 *////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

// a possible path out of my encoding trouble, but I have to do some studying.
#include "libavcodec/avcodec.h"
#include "libavutil/mathematics.h"

// notes
float C = 261.63;
float D = 296.66;
float E = 329.63;
float G = 392.00;
float A = 440.00;
float R = 0;

//sample rate
float rate = 22000;

char sampletou8(float sample) {
  sample = sample * 127 + 128;
  return (unsigned char) (unsigned int) sample;
}

float organ[8] = {1, 0.5, 0.8, 0.3, 0, 0.2, 0, 0.1};

float tremolo(float freq, float amp, float sample) {
  return (sin(6.28 * freq * amp * sample) + 1) * amp/2 + (1-amp);
}

void note(float freq, float dur) {
  float time = 0;
  //sum of the instrument's tones
  float ttl = 0; 
  for(int i = 0; i < (int) (sizeof(organ)); i++) {
    ttl += organ[i];
  }
  // plot out the thing
  while (time < dur) {
    float sample = 0;
    for(int i = 0; i < (int) (sizeof(organ)); i++) {
      sample += 0.30 / ttl * organ[i] * sin(6.28 * time * freq * (i +1));
    }

    sample *= tremolo(0.40, 4, sample);

    //printf("%f\n", sample);
    sample = sampletou8(sample);

    //const void sample;
    write(STDOUT_FILENO, &sample, sizeof(sample)-1);
    time += 1/rate;
  }
}

int main() {
  float dur = 0.2;
  note(E, dur);
  note(D, dur);
  note(C, dur);
  note(D, dur);
  note(E, dur);
  note(E, dur);
  note(E, dur);
  note(R, dur);
  note(D, dur);
  note(D, dur);
  note(D, dur);
  note(R, dur);
  note(E, dur);
  note(G, dur);
  note(G, dur);
  note(R, dur);
  note(E, dur);
  note(D, dur);
  note(C, dur);
  note(D, dur);
  note(E, dur);
  note(E, dur);
  note(E, dur);
  note(E, dur);
  note(D, dur);
  note(D, dur);
  note(E, dur);
  note(D, dur);
  note(C, dur);

  return 0;
}
