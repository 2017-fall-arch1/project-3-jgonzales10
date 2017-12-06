#include <msp430.h>
#include "libTimer.h"
#include <paddle.h>

/* advances the paddle left and right depending on cursor parameter */
void paddleAdvance(MovLayer *ml, Region *fence, int cursor)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    for (axis = 0; axis < 2; axis ++) {
      /*keeps paddle in boundary*/
      if ((shapeBoundary.topLeft.axes[0] < fence->topLeft.axes[0])){
	int velocity = 10;
	  newPos.axes[0] += (velocity);
	}
      if(shapeBoundary.botRight.axes[0] > fence->botRight.axes[0]) {
	int velocity = -10;
	newPos.axes[0] += (velocity);
      }
      /*Move paddle*/
      if(cursor == 0){
	int velocity = -5;//ml->velocity.axes[axis] = -ml->velocity.axes[axis]
	newPos.axes[0] += (velocity);
      }
      if(cursor == 1){
	int velocity = ml->velocity.axes[axis];
	newPos.axes[axis] += (velocity);
      }
      /**< if outside of fence */
    } /**< for axis */
   ml->layer->posNext = newPos;
  } /**< for ml */
}
void paddleMove(Region *fence){
  u_int switches = p2sw_read(), i;
  char str[5];
  for(i = 0; i < 4; i++){
    str[i] = (switches & (1<<i)) ? '-' : '0'+i;
  }
  str[4] = 0;
  drawString5x7(0,screenHeight-8,str,COLOR_BLUE,COLOR_BLACK);
  if(p2sw_read() == 11){
    paddleAdvance(&ml3, fence,1);
    movLayerDraw(&ml3, &paddleLayer);
    buzzer_noise1();
    noiseSwitch = 1;
  }
  if(p2sw_read() == 13){
    buzzer_noise2();
    paddleAdvance(&ml3,fence,0);
    movLayerDraw(&ml3, &paddleLayer);
    buzzer_noise2();
    noiseSwitch = 1;
  }
  if(noiseSwitch > 1){
    buzzer_noiseNull();
    noiseSwitch = 0;
  }
  noiseSwitch++;
}

