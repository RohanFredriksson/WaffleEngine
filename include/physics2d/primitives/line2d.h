#include "external.h"

#ifndef LINE2D_H
#define LINE2D_H

struct Line2D {
    vec2 from;
    vec2 to;
};
typedef struct Line2D Line2D;

void Line2D_Init(Line2D* l, vec2 from, vec2 to);

#endif