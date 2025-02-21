#ifndef ENVELOPEDETECTOR_H
#define ENVELOPEDETECTOR_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "IIRFilter.h"

/* A peak-based envelope detector
 * */
class EnvelopeDetector {
private:
  CascadeIIR inputFilter;
  CascadeIIR outputFilter;
  float prevPhasePeak = 0.0f;
  float currentPeak = 0.0f;

public:
  EnvelopeDetector();
  // call this in the ProcessSpec() callback
  void prepare(double sampleRate);
  // process a single audio input sample and return the current envelope level
  float process(float input);
};

//=======================================================
#define RMS_MAX_WINDOW 4000
#define RMS_MIN_WINDOW 800
#define RMS_DEFAULT_WINDOW 1000

class RMSMeter {
private:
  float prevValue = 0.0f;
  int samplesInSum = 0;
  float squareSum = 0.0f;

public:
  int window = RMS_DEFAULT_WINDOW;
  RMSMeter() = default;
  float process(float input);
};

#endif
