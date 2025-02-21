#ifndef MATH_HELPERS_H_
#define MATH_HELPERS_H_

namespace Math {
// https://en.wikipedia.org/wiki/Fast_inverse_square_root
static float fastInvSqrt(float x) noexcept {
  constexpr float threeHalves = 1.5f;
  float x2 = x * 0.5f;
  // it's called we do a little bit hacking
  long i = *(long*)&x;
  i = 0x05F3759DF - (i >> 1);
  x = *(float*)&i;
  x = x * (threeHalves - (x2 * x * x));
  return x;
}

static constexpr float fastSqrt(float x) {
  return fastInvSqrt(x) * x;
}
}  // namespace Math
#endif
