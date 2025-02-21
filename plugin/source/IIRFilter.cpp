#include "Multipressor/IIRFilter.h"

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
