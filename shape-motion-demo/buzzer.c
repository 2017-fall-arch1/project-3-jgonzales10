#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"

static unsigned int period = 1000;
static unsigned int softness = 12; //(in/dec)reases volume
static unsigned int curr = 0; //traverses song array
static unsigned int curr2 = 0;
static unsigned int verse2 = 0; //after certain checks it changes the song

#define MIN_PERIOD 1000
#define MAX_PERIOD 2000

void buzzer_init()
{
    /* 
       Direct timer A output "TA0.1" to P2.6.  
        According to table 21 from data sheet:
          P2SEL2.6, P2SEL2.7, anmd P2SEL.7 must be zero
          P2SEL.6 must be 1
        Also: P2.6 direction must be output
    */
    timerAUpmode();		/* used to drive speaker */
    P2SEL2 &= ~(BIT6 | BIT7);
    P2SEL &= ~BIT7; 
    P2SEL |= BIT6;
    P2DIR = BIT6;		/* enable output to speaker (P2.6) */
    //buzzer_song();
}
void buzzer_noiseNull(){
  period = 0;
  buzzer_set_period(period,8);
}
void buzzer_noise1(int volume){
    period = 3400;
    buzzer_set_period(period,volume);
}
void buzzer_noise2(int volume){
  period = 2600;
  buzzer_set_period(period,volume);
}
void buzzer_noise3(){
  for(int i = 0; i < 10; i++){
    period = 2600;
    buzzer_set_period(period,4);
  }
}
void buzzer_noise4(){
  period = 2200;
  buzzer_set_period(period,2);
}
void buzzer_set_period(short cycles, int softness)
{
  CCR0 = cycles; 
  CCR1 = cycles >> softness;		/* one half cycle */
}
