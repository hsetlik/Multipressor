#include "Multipressor/IIRFilter.h"
#include "juce_core/juce_core.h"

SingleIIR::SingleIIR() : core(nullptr) {}

void SingleIIR::setParams(single_iir_params_t other) {
  params = other;
  if (filterPrepared)
    prepareFilter();
}

void SingleIIR::prepare(double sr) {
  sampleRate = sr;
  prepareFilter();
  filterPrepared = true;
}

float SingleIIR::process(float input) {
  return core->processSample(input);
}

void SingleIIR::prepareFilter() {
  iir_type_t type = (iir_type_t)params.filterType;
  switch (type) {
    case FirstOrderLowPass:
      core.reset(new iir_core_t(
          iir_coeffs_t::makeFirstOrderLowPass(sampleRate, params.cutoff)));
      break;
    case FirstOrderHighPass:
      core.reset(new iir_core_t(
          iir_coeffs_t::makeFirstOrderHighPass(sampleRate, params.cutoff)));
      break;
    case FirstOrderAllPass:
      core.reset(new iir_core_t(
          iir_coeffs_t::makeFirstOrderAllPass(sampleRate, params.cutoff)));
      break;
    case NormalLowPass:
      core.reset(
          new iir_core_t(iir_coeffs_t::makeLowPass(sampleRate, params.cutoff)));
      break;
    case NormalHighPass:
      core.reset(new iir_core_t(
          iir_coeffs_t::makeHighPass(sampleRate, params.cutoff)));
      break;
    case NormalBandPass:
      core.reset(new iir_core_t(
          iir_coeffs_t::makeBandPass(sampleRate, params.cutoff)));
      break;
    case NormalAllPass:
      core.reset(
          new iir_core_t(iir_coeffs_t::makeAllPass(sampleRate, params.cutoff)));
      break;
    case NormalNotch:
      core.reset(
          new iir_core_t(iir_coeffs_t::makeNotch(sampleRate, params.cutoff)));
      break;
    case LowPassQ:
      core.reset(new iir_core_t(
          iir_coeffs_t::makeLowPass(sampleRate, params.cutoff, params.q)));
      break;
    case HighPassQ:
      core.reset(new iir_core_t(
          iir_coeffs_t::makeHighPass(sampleRate, params.cutoff, params.q)));
      break;
    case BandPassQ:
      core.reset(new iir_core_t(
          iir_coeffs_t::makeBandPass(sampleRate, params.cutoff, params.q)));
      break;
    case AllPassQ:
      core.reset(new iir_core_t(
          iir_coeffs_t::makeAllPass(sampleRate, params.cutoff, params.q)));
      break;
    case NotchQ:
      core.reset(new iir_core_t(
          iir_coeffs_t::makeNotch(sampleRate, params.cutoff, params.q)));
      break;
    case LowShelf:
      core.reset(new iir_core_t(iir_coeffs_t::makeLowShelf(
          sampleRate, params.cutoff, params.q, params.gain)));
      break;
    case HighShelf:
      core.reset(new iir_core_t(iir_coeffs_t::makeHighShelf(
          sampleRate, params.cutoff, params.q, params.gain)));
      break;
    case Peak:
      core.reset(new iir_core_t(iir_coeffs_t::makePeakFilter(
          sampleRate, params.cutoff, params.q, params.gain)));
      break;
  }
}

//=================================================================================

void CascadeIIR::setParams(cascade_iir_params_t other) {
  params = other;
  if (filtersPrepared)
    prepareCascade();
}

void CascadeIIR::prepare(double sr) {
  sampleRate = sr;
  prepareCascade();
  filtersPrepared = true;
}

float CascadeIIR::process(float input) {
  float out = input;
  for (int i = 0; i < filters.size(); i++) {
    out = filters[i]->processSample(out);
  }
  return out;
}

void CascadeIIR::prepareCascade() {
  // 1. use the functions from dsp::FilterDesign to generate a
  // list of coeffs
  juce::ReferenceCountedArray<iir_coeffs_t> arr;
  iir_cascade_t type = (iir_cascade_t)params.filterType;
  switch (type) {
    case ButterworthLowPass1:
      arr = cascade_d::designIIRLowpassHighOrderButterworthMethod(
          params.cutoff, sampleRate, params.order);
      break;
    case ButterworthLowPass2:
      arr = cascade_d::designIIRLowpassHighOrderButterworthMethod(
          params.cutoff, sampleRate, params.transitionWidth,
          params.passbandGain, params.stopbandGain);
      break;
    case ButterworthHighPass:
      arr = cascade_d::designIIRHighpassHighOrderButterworthMethod(
          params.cutoff, sampleRate, params.order);
      break;
    case Chebyshev1LowPass:
      arr = cascade_d::designIIRLowpassHighOrderChebyshev1Method(
          params.cutoff, sampleRate, params.transitionWidth,
          params.passbandGain, params.stopbandGain);
      break;
    case Chebyshev2LowPass:
      arr = cascade_d::designIIRLowpassHighOrderChebyshev2Method(
          params.cutoff, sampleRate, params.transitionWidth,
          params.passbandGain, params.stopbandGain);
      break;
    case EllipticLowPass:
      arr = cascade_d::designIIRLowpassHighOrderEllipticMethod(
          params.cutoff, sampleRate, params.transitionWidth,
          params.passbandGain, params.stopbandGain);
      break;
    default:
      break;
  }

  // 2. repopulate the OwnedArray with the new filters
  filters.clear();
  for (int i = 0; i < arr.size(); i++) {
    filters.add(new iir_core_t(arr[i]));
  }
}
