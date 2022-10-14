#ifndef GEOMETRY_H
#define GEOMETRY_H

float Geometry_PerpendicularDistance(vec2 start, vec2 stop, vec2 point);
bool Geometry_IsAboveLine(vec2 start, vec2 stop, vec2 point);
bool Geometry_IsBelowLine(vec2 start, vec2 stop, vec2 point);
bool Geometry_IsLeftOfLine(vec2 start, vec2 stop, vec2 point);
bool Geometry_IsRightOfLine(vec2 start, vec2 stop, vec2 point);

#endif