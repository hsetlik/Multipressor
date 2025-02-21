#ifndef ENVELOPEDETECTOR_H
#define ENVELOPEDETECTOR_H

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

// bc the juce names are a mouthful
typedef juce::dsp::IIR::Filter<float> IIR_floatFilter;

class EnvelopeDetector {
private:
  IIR_floatFilter inputFilter;
  IIR_floatFilter outputFilter;
  float prevPhasePeak = 0.0f;
  float currentPeak = 0.0f;

public:
  EnvelopeDetector();
  // call this in the ProcessSpec() callback
  void prepare(double sampleRate);
  // process a single audio input sample and return the current envelope level
  float process(float input);
};

#endif
