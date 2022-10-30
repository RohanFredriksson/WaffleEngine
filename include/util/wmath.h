#include <stdbool.h>
#include "external.h"

#ifndef WMATH_H
#define WMATH_H

int WMath_MaxInt(int a, int b);
int WMath_MinInt(int a, int b);
float WMath_MaxFloat(float a, float b);
float WMath_MinFloat(float a, float b);
bool WMath_CompareFloat(float a, float b);
bool WMath_CompareVec2(vec2 a, vec2 b);
bool WMath_CompareFloatEpsilon(float a, float b, float epsilon);
bool WMath_CompareVec2Epsilon(vec2 a, vec2 b, float epsilon);

void WMath_Rotate(vec2 vec, float angleDeg, vec2 origin);


#endif