/** \file shapemotion.c
 *  \brief This is a simple shape motion demo.
 *  This demo creates two layers containing shapes.
 *  One layer contains a rectangle and the other a circle.
 *  While the CPU is running the green LED is on, and
 *  when the screen does not need to be redrawn the CPU
 *  is turned off along with the green LED.
 */

#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>
#include <chordVec.h>
#include <buzzer.h>

#define GREEN_LED BIT6

static unsigned int points = 0; //keeps track of points scored
char snum[5];
static unsigned int count = 0; //increments to change title
static unsigned int soundLength = 0;
static unsigned int noiseSwitch = 0; //switches the buzzer noise
static unsigned int lives = 9;
static unsigned int volume = 1;
/* creates circle with missing piece, similar to a copyrighted character */
int
abSlicedCircleCheck(const AbCircle *circle, const Vec2 *centerPos, const Vec2 *pixel)
{
  
  Vec2 relPos;
  vec2Sub(&relPos, pixel, centerPos); /* vector from center to pixel */
  if(relPos.axes[0] >= 0 && relPos.axes[0] <= relPos.axes[1]){
    return 0;
  }
  else
    return abCircleCheck(circle, centerPos, pixel);
}

/* define the playing shapes */
AbRect floorBoundary = {abRectGetBounds, abRectCheck, {(58), 1}};
AbCircle circleBad = {abCircleGetBounds, abSlicedCircleCheck, chordVec10, 10};
AbCircle circleRed = {abCircleGetBounds, abSlicedCircleCheck, chordVec10, 10};
AbRect pongRect = {abRectGetBounds, abRectCheck, {15,2}}; /**< 15x2 rectangle*/

AbRectOutline fieldOutline = {	/* playing field */
  abRectOutlineGetBounds, abRectOutlineCheck,   
  {(screenWidth/2)-6, (screenHeight/2)}
};
Layer floorLayer = {
  (AbShape *)&floorBoundary,
  {screenWidth/2, (screenHeight-10)},
  {0,0}, {0,0},
  COLOR_RED,
  0
};

Layer paddleLayer = {
  (AbShape *)&pongRect,
  {(screenWidth/2), (screenHeight-20)}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_WHITE,
  &floorLayer,
};
  

Layer layer3 = {		/**< Layer with an orange circle */
  (AbShape *)&circleRed,
  {(screenWidth/2)+10, (screenHeight/2)+5}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_RED,
  &paddleLayer,
};

Layer fieldLayer = {		/* playing field as a layer */
  (AbShape *) &fieldOutline,
  {screenWidth/2, screenHeight/2+10},/**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_BLUE,
  &layer3
};

Layer layer1 = {		/**< Default (white)Ball Layer */
  (AbShape *)&circle5,
  {(screenWidth/2), (screenHeight/2)-14}, /**< center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_WHITE,
  &fieldLayer,
};

Layer layer0 = {		/*< Layer with an orange circle */
  (AbShape *)&circleBad,
  {(screenWidth/2)+10, (screenHeight/2)+5}, /*< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_YELLOW,
  &layer1,
};

/** Moving Layer
 *  Linked list of layer references
 *  Velocity represents one iteration of change (direction & magnitude) */

typedef struct MovLayer_s {
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;

/* initial value of {0,0} will be overwritten */
//MovLayer ml3 = { &layer4, {4,0}, 0}; /**< not all layers move */
MovLayer ml4 = { &floorLayer, {0,0}, 0};
MovLayer ml3 = { &paddleLayer, {4,0}, 0};
MovLayer ml2 = { &layer1, {3,3}, 0};
MovLayer ml1 = { &layer0, {2,3}, &ml2 }; 
MovLayer ml0 = { &layer3, {3,2}, &ml1 }; 

void movLayerDraw(MovLayer *movLayers, Layer *layers)
{
  int row, col;
  MovLayer *movLayer;

  and_sr(~8);			/**< disable interrupts (GIE off) */
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);			/**< disable interrupts (GIE on) */


  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], 
		bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
	Vec2 pixelPos = {col,row};
	u_int color = bgColor;
	Layer *probeLayer;
	for (probeLayer = layers; probeLayer; 
	     probeLayer = probeLayer->next) { /* probe all layers, in order */
	  if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
	    color = probeLayer->color;
	    break; 
	  } /* if probe check */
	} // for checking all layers at col, row
	lcd_writeColor(color); 
      } // for col
    } // for row
  } // for moving layer being updated
}	  



//Region fence = {{10,30}, {SHORT_EDGE_PIXELS-10, LONG_EDGE_PIXELS-10}}; /**< Create a fence region */

/** Advances a moving shape within a fence
 *  
 *  \param ml The moving shape to be advanced
 *  \param fence The region which will serve as a boundary for ml
 */
void mlAdvance(MovLayer *ml, Region *fence)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
    for (axis = 0; axis < 2; axis ++) {
      fieldLayer.pos.axes[1] +=50;
      if ((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) ||
	  (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis])) {
	//fieldLayer.pos.axes[1] -= 50;
	int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
	newPos.axes[axis] += (velocity);
      }
      if(abRectCheck(&floorBoundary, &(floorLayer.pos),
		     &(ml->layer->pos)) && axis ==1){
	int velocity = -100;
	lives--;
	newPos.axes[axis] += (velocity);
      }
      /**< if outside of fence */
      if(abRectCheck(&pongRect,&(paddleLayer.pos),
		       &(ml->layer->pos)) && axis==1){ 
	points++;
        int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
        newPos.axes[axis] += (4*velocity);
      }
      fieldLayer.pos.axes[1] -= 50;
    } /**< for axis */
    ml->layer->posNext = newPos;
  } /**< for ml */
}

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
  //char* pnum = itoa(p2sw_read(),snum,10);
  //drawString5x7(screenWidth/2,screenHeight/2, pnum , COLOR_GREEN, COLOR_BLACK); //score implem.
  drawString5x7(0,screenHeight-8,str,COLOR_BLUE,COLOR_BLACK);
  if((p2sw_read() == 14) && (volume <= 10)){ //switch 1 decreases noise
    
    volume++;
  }
  if((p2sw_read() == 75) && (volume >= 1)){ //switch 4 increases noise
    volume--;
  }
  if(p2sw_read() == 11){ //switch 2 moves paddle to left
    paddleAdvance(&ml3, fence,1);
    movLayerDraw(&ml3, &paddleLayer);
    buzzer_noise1(volume);
    noiseSwitch = 1;
  }
  if(p2sw_read() == 13){ //switch 3 moves paddle to right
    buzzer_noise2();
    paddleAdvance(&ml3,fence,0);
    movLayerDraw(&ml3, &paddleLayer);
    buzzer_noise2(volume);
    noiseSwitch = 1;
  }
  if(noiseSwitch > 1){ //just made to shut up the buzzer
    buzzer_noiseNull();
    noiseSwitch = 0;
  }
  if(volume == 11){
    volume = 1;
  }
  noiseSwitch++;
}

void titleDraw(){ /*title "Feed Reddy Freddy" alternates on top right */
  
    if(count < 4){
      //drawString5x7((screenHeight/2)-4,0, "      ", COLOR_BLACK, COLOR_BLACK);
      drawString5x7((screenHeight/2)-4, 0, "PONG", COLOR_YELLOW, COLOR_BLACK);
      count++;
    }
    if(count >= 4 && count < 10){
      drawString5x7((screenHeight/2)-4,0, "PONG", COLOR_RED, COLOR_BLACK);
      count ++;
    }
    if(count == 10){
      count = 0;
    }
    
    char* pnum = itoa(points,snum,10);
    drawString5x7(0,0, "Score:", COLOR_GREEN, COLOR_BLACK); //score implem.
    drawString5x7((screenHeight/4),0, pnum,
    		  COLOR_GREEN, COLOR_BLACK);
    pnum = itoa(lives, snum, 10);
    /* prints number of lives left */
    drawString5x7(50, (screenHeight-8), "Lives: ", COLOR_WHITE, COLOR_BLACK);
    drawString5x7(90,(screenHeight-8),pnum, COLOR_RED, COLOR_BLACK);
}

u_int bgColor = COLOR_BLACK;     /**< The background color */
int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

Region fieldFence;		/**< fence around playing field  */


/** Initializes everything, enables interrupts and green LED, 
 *  and handles the rendering for the screen
 */
void main()
{
  P1DIR |= GREEN_LED;		/**< Green led on when CPU on */		
  P1OUT |= GREEN_LED;

  configureClocks();
  lcd_init();
  shapeInit();
  p2sw_init(15);
  shapeInit();
  buzzer_init();
  layerInit(&layer0);
  layerDraw(&layer0);


  layerGetBounds(&fieldLayer, &fieldFence); /*region around field (where shapes bump */
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */

  for(;;){
    titleDraw();
    paddleMove(&fieldFence);

  //for(;;) { 
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
    		  
    P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
    or_sr(0x10);	      /**< CPU OFF */
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    redrawScreen = 0;
    movLayerDraw(&ml0, &layer0);
  }
}

/** Watchdog timer interrupt handler. 15 interrupts/sec */
void wdt_c_handler()
{
  static short count = 0;
  P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
  count ++;
  if (count == 20) {
    mlAdvance(&ml0, &fieldFence);
    if (p2sw_read())
      redrawScreen = 1;
    count = 0;
  } 
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}
