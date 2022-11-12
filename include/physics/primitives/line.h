#include "external.h"

#ifndef LINE_H
#define LINE_H

struct Line {
    vec2 from;
    vec2 to;
};
typedef struct Line Line;

void Line_Init(Line* l, vec2 from, vec2 to);
float Line_LengthSquared(Line* l);

#endif