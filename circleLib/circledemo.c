#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include "abCircle.h"
#include "chordVec.h"

AbRect rect10 = {abRectGetBounds, abRectCheck, {10,10}};; /**< 10x10 rectangle */

int
abSlicedCircleCheck(const AbCircle *circle, const Vec2 *centerPos, const Vec2 *pixel)
{
  u_char radius = circle->radius;
  int axis;
  Vec2 relPos;
  vec2Sub(&relPos, pixel, centerPos); /* vector from center to pixel */
  //vec2Add(&relPos, pixel, centerPos);
  //vec2Abs(&relPos); //makes the shape congruent
  //vec2Min(&relPos, pixel, centerPos);
  if(relPos.axes[0] >= 0 && relPos.axes[0] <= relPos.axes[1]){
    return 0;
  }
  else
    return abCircleCheck(circle, centerPos, pixel);
}

u_int bgColor = COLOR_BLUE;

AbCircle circleC = {abCircleGetBounds, abSlicedCircleCheck, chordVec50, 50};

Layer layer1 = {		/**< Layer with a red square */
  (AbShape *)&rect10,
  {screenWidth/4, screenHeight/2}, /**< center */
  {0,0}, {0,0},				    /* next & last pos */
  COLOR_RED,
  0
};

Layer layer0 = {		/**< Layer with an orange circle */
  (AbCircle *)&circleC,
  {(screenWidth/2), (screenHeight/2)}, /**< bit below & right of center */
  {0,0},{0,0},				    /* next & last pos */
  COLOR_ORANGE,
  &layer1,
};

int
main()
{
  configureClocks();
  lcd_init();
  shapeInit();
  Vec2 rectPos = screenCenter, centerPos = {30,screenHeight - 30};
  
  clearScreen(COLOR_BLACK);
  drawString5x7(20,20, "!!!!OLIVER!!!!", COLOR_WHITE, COLOR_RED);
  shapeInit();
  
  //drawString10x14(40,40, "OLIVER", COLOR_GREEN, COLOR_RED);
  layerDraw(&layer0);

}
