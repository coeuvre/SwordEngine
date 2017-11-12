#ifndef SD_MATH_H
#define SD_MATH_H

// ----------------------------------------------------------------------------
// Vector 2
// ----------------------------------------------------------------------------

typedef struct Vec2 {
  float x, y;
} Vec2;

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
typedef struct Mat3 {
  float m00, m10, m20;
  float m01, m11, m21;
  float m02, m12, m22;
} Mat3;

// ----------------------------------------------------------------------------
// Rectangle
// ----------------------------------------------------------------------------

typedef struct Rect {
  Vec2 min, max;
} Rect;

#endif  // SD_MATH_H
