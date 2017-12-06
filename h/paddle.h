#ifndef paddle_included
#define paddle_included
#include "shape.h"
#include "buzzer.h"

void paddleAdvance(const MovLayer *ml, const Region *fence, const int cursor);
void paddleMove(const Region *fence);
#endif
