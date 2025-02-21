#include "Multipressor/EnvelopeDetector.h"
#include "Multipressor/IIRFilter.h"

EnvelopeDetector::EnvelopeDetector() {
  // initialize the filters here
  cascade_iir_params_t inParams;
  inParams.filterType = iir_cascade_t::ButterworthLowPass1;
  inParams.order = 4;
  inParams.cutoff = 850.0f;
  cascade_iir_params_t outParams;
  outParams.filterType = iir_cascade_t::ButterworthLowPass1;
  outParams.order = 4;
  outParams.cutoff = 50.0f;

  inputFilter.setParams(inParams);
  outputFilter.setParams(outParams);
}

void EnvelopeDetector::prepare(double sampleRate) {
  inputFilter.prepare(sampleRate);
  outputFilter.prepare(sampleRate);
}

float EnvelopeDetector::process(float raw) {
  float input = inputFilter.process(raw);

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
  return outputFilter.process(prevPhasePeak);
}
