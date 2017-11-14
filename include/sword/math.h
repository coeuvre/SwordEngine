#ifndef SD_MATH_H
#define SD_MATH_H

#include "sword/def.h"

#include <math.h>

// ----------------------------------------------------------------------------
// Scalar
// ----------------------------------------------------------------------------

typedef float SDFloat;

SDINLINE SDFloat SDMinF(SDFloat x, SDFloat y) { return x <= y ? x : y; }

SDINLINE SDFloat SDAbsF(SDFloat x) { return fabsf(x); }

SDINLINE SDFloat SDFloorF(SDFloat x) { return floorf(x); }

SDINLINE SDFloat SDCeilF(SDFloat x) { return ceilf(x); }

SDINLINE SDFloat SDClampF(SDFloat x, SDFloat min, SDFloat max) {
  SDFloat result = x;

  if (x < min) {
    result = min;
  } else if (x > max) {
    result = max;
  }

  return result;
}

SDINLINE SDFloat SDClamp01F(SDFloat x) { return SDClampF(x, 0.0f, 1.0f); }

SDINLINE SDFloat SDLerpF(SDFloat a, SDFloat t, SDFloat b) {
  return (1.0f - t) * a + t * b;
}

SDINLINE SDFloat SDNextPow2F(SDFloat x) { return powf(2.0f, SDCeilF(log2f(x))); }

// ----------------------------------------------------------------------------
// Vector2
// ----------------------------------------------------------------------------

typedef struct SDVec2 {
  SDFloat x, y;
} SDVec2;

// ----------------------------------------------------------------------------
// Matrix 3
// ----------------------------------------------------------------------------

// The affine transform matrix:
//
//     | a c x |    | x y o |
//     | b d y | or | x y o |
//     | 0 0 1 |    | x y o |
//
// This matrix is used to multiply by column vector:
//
//     | a c x |   | x |
//     | b d y | * | y |
//     | 0 0 1 |   | 1 |
//
// This matrix use column-major order to store elements
typedef struct SDMat3 {
  SDFloat m00, m10, m20;
  SDFloat m01, m11, m21;
  SDFloat m02, m12, m22;
} SDMat3;

// ----------------------------------------------------------------------------
// Rectangle
// ----------------------------------------------------------------------------

typedef struct SDRect {
  SDVec2 min, max;
} SDRect;

#endif  // SD_MATH_H
