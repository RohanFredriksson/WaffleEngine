#include <math.h>
#include <float.h>
#include "wmath.h"

int WMath_MaxInt(int a, int b) {
    return (a > b) ? a : b;
}

int WMath_MinInt(int a, int b) {
    return (a > b) ? b : a;
}

float WMath_MaxFloat(float a, float b) {
    return (a > b) ? a : b;
}

float WMath_MinFloat(float a, float b) {
    return (a > b) ? b : a;
}

bool WMath_CompareFloat(float a, float b) {
    return fabsf(a - b) <= FLT_MIN * WMath_MaxFloat(1.0f, WMath_MaxFloat(fabsf(a), fabsf(b)));
}

bool WMath_CompareVec2(vec2 a, vec2 b) {
    return WMath_CompareFloat(a[0], b[0]) && WMath_CompareFloat(a[1], b[1]);
}

bool WMath_CompareFloatEpsilon(float a, float b, float epsilon) {
    return fabsf(a - b) <= epsilon * WMath_MaxFloat(1.0f, WMath_MaxFloat(fabsf(a), fabsf(b)));
}

bool WMath_CompareVec2Epsilon(vec2 a, vec2 b, float epsilon) {
    return WMath_CompareFloatEpsilon(a[0], b[0], epsilon) && WMath_CompareFloatEpsilon(a[1], b[1], epsilon);
}

void WMath_Rotate(vec2 vec, float angleDeg, vec2 origin) {
    
    float x = vec[0] - origin[0];
    float y = vec[1] - origin[1];

    float cos = cosf(angleDeg * (GLM_PI / 180));
    float sin = sinf(angleDeg * (GLM_PI / 180));

    float xPrime = (x * cos) - (y * sin);
    float yPrime = (x * sin) + (y * cos);
    
    xPrime += origin[0];
    yPrime += origin[1];

    vec[0] = xPrime;
    vec[1] = yPrime;

}

