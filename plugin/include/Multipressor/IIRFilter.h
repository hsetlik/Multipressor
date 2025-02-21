#ifndef _IIRFILTER_H
#define _IIRFILTER_H
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <memory.h>

typedef juce::dsp::IIR::Filter<float> iir_core_t;
typedef juce::dsp::IIR::Coefficients<float> iir_coeffs_t;

// enum for the different IIF filter topologies
// provided in the dsp::FilterDesign juce module
// https://docs.juce.com/master/structdsp_1_1FilterDesign.html

enum iir_type_t {
  FirstOrderLowPass,
  FirstOrderHighPass,
  FirstOrderAllPass,
  NormalLowPass,
  NormalHighPass,
  NormalBandPass,
  NormalNotch,
  NormalAllPass,
  LowPassQ,
  HighPassQ,
  BandPassQ,
  NotchQ,
  AllPassQ,
  LowShelf,
  HighShelf,
  Peak
};

#define NUM_IIR_TYPES 16

struct single_iir_params_t {
  uint8_t filterType = iir_type_t::NormalLowPass;
  float cutoff = 2000.0f;
  float q = 0.0f;
  float gain = 1.0f;
  single_iir_params_t& operator=(const single_iir_params_t& other) {
    filterType = other.filterType;
    cutoff = other.cutoff;
    q = other.q;
    gain = other.gain;
    return *this;
  }
};

class SingleIIR {
private:
  std::unique_ptr<iir_core_t> core;
  single_iir_params_t params;
  double sampleRate;
  bool filterPrepared = false;
  // helper for re-setting up the unique_ptr
  void prepareFilter();

public:
  SingleIIR();
  void setParams(single_iir_params_t other);
  void prepare(double sampleFreq);
  float process(float input);
};

#endif
