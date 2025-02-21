#include "Multipressor/EnvelopeDetector.h"

EnvelopeDetector::EnvelopeDetector() {}

void EnvelopeDetector::prepare(double sampleRate) {
  auto inputCoeffs = juce::dsp::FilterDesign<
      float>::designIIRLowpassHighOrderButterworthMethod(650.0f, sampleRate, 4);

  auto outputCoeffs = juce::dsp::FilterDesign<
      float>::designIIRLowpassHighOrderButterworthMethod(45.0f, sampleRate, 4);
}

float EnvelopeDetector::process(float input) {
  // TODO: the input should get filtered here

  // check for a zero crossing
  static float prevInput = 0.0f;
  if ((prevInput < 0.0f) != (input < 0.0f)) {
    prevPhasePeak = currentPeak;
    currentPeak = 0.0f;
  }
  prevInput = input;
  input = std::fabs(input);
  if (input > currentPeak) {
    currentPeak = input;
  }
  // TODO: output filter
  return prevPhasePeak;
}
