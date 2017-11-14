#ifndef SD_MATH_H
#define SD_MATH_H

typedef float SDFloat;

// ----------------------------------------------------------------------------
// Vector 2
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
