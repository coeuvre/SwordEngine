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

SDINLINE SDFloat SDNextPow2F(SDFloat x) {
  return powf(2.0f, SDCeilF(log2f(x)));
}

// ----------------------------------------------------------------------------
// Vector2
// ----------------------------------------------------------------------------

typedef struct SDVec2 {
  SDFloat x, y;
} SDVec2;

SDINLINE SDVec2 SDV2(SDFloat x, SDFloat y) { return (SDVec2){x, y}; }

SDINLINE SDVec2 SDHadamardDivV2(SDVec2 a, SDVec2 b) {
  return (SDVec2){a.x / b.x, a.y / b.y};
}

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

SDINLINE SDMat3 SDIdentityM3(void) {
  SDMat3 result;

  result.m00 = 1.0f;
  result.m01 = 0.0f;
  result.m02 = 0.0f;

  result.m10 = 0.0f;
  result.m11 = 1.0f;
  result.m12 = 0.0f;

  result.m20 = 0.0f;
  result.m21 = 0.0f;
  result.m22 = 1.0f;

  return result;
}

SDINLINE SDMat3 SDMat3Translation(SDFloat x, SDFloat y) {
  SDMat3 result = SDIdentityM3();
  result.m02 = x;
  result.m12 = y;
  return result;
}

SDINLINE SDMat3 SDMat3Rotation(SDFloat rad) {
  SDMat3 result = {0};
  result.m00 = cosf(rad);
  result.m10 = sinf(rad);
  result.m01 = -result.m10;
  result.m11 = result.m00;
  return result;
}

SDINLINE SDMat3 SDMat3Scale(SDFloat sx, SDFloat sy) {
  SDMat3 result = SDIdentityM3();
  result.m00 = sx;
  result.m11 = sy;
  return result;
}

SDINLINE SDMat3 SDDotM3(SDMat3 t1, SDMat3 t2) {
  SDMat3 result;

  result.m00 = t1.m00 * t2.m00 + t1.m01 * t2.m10 + t1.m02 * 0.0f;
  result.m01 = t1.m00 * t2.m01 + t1.m01 * t2.m11 + t1.m02 * 0.0f;
  result.m02 = t1.m00 * t2.m02 + t1.m01 * t2.m12 + t1.m02 * 1.0f;

  result.m10 = t1.m10 * t2.m00 + t1.m11 * t2.m10 + t1.m12 * 0.0f;
  result.m11 = t1.m10 * t2.m01 + t1.m11 * t2.m11 + t1.m12 * 0.0f;
  result.m12 = t1.m10 * t2.m02 + t1.m11 * t2.m12 + t1.m12 * 1.0f;

  result.m20 = 0.0f;
  result.m21 = 0.0f;
  result.m22 = 1.0f;

  return result;
}

// ----------------------------------------------------------------------------
// Rectangle
// ----------------------------------------------------------------------------

typedef struct SDRect {
  SDVec2 min, max;
} SDRect;

SDINLINE SDRect SDRectMinMax(SDVec2 min, SDVec2 max) {
  return (SDRect){min, max};
}

#endif  // SD_MATH_H
