#ifndef abCircle_included
#define abCircle_included

#include "shape.h"

/** AbShape circle
 *  
 *  chords should be a vector of length radius + 1.  
 *  Entry at index i is 1/2 chord length at distance i from the circle's center.  
 *  This vector can be generated using lcdLib's computeChordVec() (lcddraw.h).
 */ 
typedef struct AbCircle_s {
  void (*getBounds)(const struct AbCircle_s *circle, const Vec2 *centerPos, Region *bounds);
  int (*check)(const struct AbCircle_s *circle, const Vec2 *centerPos, const Vec2 *pixel);
  const int mouthSize;
  const u_char *chords;
  const u_char radius;
} AbCircle;

/** Required by AbShape
 */
void abCircleGetBounds(const AbCircle *circle, const Vec2 *circlePos, Region *bounds);

/** Required by AbShape
 */
int abCircleCheck(const AbCircle *circle, const Vec2 *circlePos, const Vec2 *pixel);

#endif



// Automatically generated by makeCircles.  (c) Eric Freudenthal, 2016
#ifndef abCircle_decls_included
#define abCircle_decls_included

extern const AbCircle circle2;
extern const AbCircle circle3;
extern const AbCircle circle4;
extern const AbCircle circle5;
extern const AbCircle circle6;
extern const AbCircle circle7;
extern const AbCircle circle8;
extern const AbCircle circle9;
extern const AbCircle circle10;
extern const AbCircle circle11;
extern const AbCircle circle12;
extern const AbCircle circle13;
extern const AbCircle circle14;
extern const AbCircle circle15;
extern const AbCircle circle16;
extern const AbCircle circle17;
extern const AbCircle circle18;
extern const AbCircle circle19;
extern const AbCircle circle20;
extern const AbCircle circle21;
extern const AbCircle circle22;
extern const AbCircle circle23;
extern const AbCircle circle24;
extern const AbCircle circle25;
extern const AbCircle circle26;
extern const AbCircle circle27;
extern const AbCircle circle28;
extern const AbCircle circle29;
extern const AbCircle circle30;
extern const AbCircle circle31;
extern const AbCircle circle32;
extern const AbCircle circle33;
extern const AbCircle circle34;
extern const AbCircle circle35;
extern const AbCircle circle36;
extern const AbCircle circle37;
extern const AbCircle circle38;
extern const AbCircle circle39;
extern const AbCircle circle40;
extern const AbCircle circle41;
extern const AbCircle circle42;
extern const AbCircle circle43;
extern const AbCircle circle44;
extern const AbCircle circle45;
extern const AbCircle circle46;
extern const AbCircle circle47;
extern const AbCircle circle48;
extern const AbCircle circle49;
extern const AbCircle circle50;
extern const AbCircle circle51;
extern const AbCircle circle52;
extern const AbCircle circle53;
extern const AbCircle circle54;
extern const AbCircle circle55;
extern const AbCircle circle56;
extern const AbCircle circle57;
extern const AbCircle circle58;
extern const AbCircle circle59;
extern const AbCircle circle60;
extern const AbCircle circle61;
extern const AbCircle circle62;
extern const AbCircle circle63;
extern const AbCircle circle64;
extern const AbCircle circle65;
extern const AbCircle circle66;
extern const AbCircle circle67;
extern const AbCircle circle68;
extern const AbCircle circle69;
extern const AbCircle circle70;
extern const AbCircle circle71;
extern const AbCircle circle72;
extern const AbCircle circle73;
extern const AbCircle circle74;
extern const AbCircle circle75;
extern const AbCircle circle76;
extern const AbCircle circle77;
extern const AbCircle circle78;
extern const AbCircle circle79;
extern const AbCircle circle80;
extern const AbCircle circle81;
extern const AbCircle circle82;
extern const AbCircle circle83;
extern const AbCircle circle84;
extern const AbCircle circle85;
extern const AbCircle circle86;
extern const AbCircle circle87;
extern const AbCircle circle88;
extern const AbCircle circle89;
extern const AbCircle circle90;
extern const AbCircle circle91;
extern const AbCircle circle92;
extern const AbCircle circle93;
extern const AbCircle circle94;
extern const AbCircle circle95;
extern const AbCircle circle96;
extern const AbCircle circle97;
extern const AbCircle circle98;
extern const AbCircle circle99;
extern const AbCircle circle100;
extern const AbCircle circle101;
extern const AbCircle circle102;
extern const AbCircle circle103;
extern const AbCircle circle104;
extern const AbCircle circle105;
extern const AbCircle circle106;
extern const AbCircle circle107;
extern const AbCircle circle108;
extern const AbCircle circle109;
extern const AbCircle circle110;
extern const AbCircle circle111;
extern const AbCircle circle112;
extern const AbCircle circle113;
extern const AbCircle circle114;
extern const AbCircle circle115;
extern const AbCircle circle116;
extern const AbCircle circle117;
extern const AbCircle circle118;
extern const AbCircle circle119;
extern const AbCircle circle120;
extern const AbCircle circle121;
extern const AbCircle circle122;
extern const AbCircle circle123;
extern const AbCircle circle124;
extern const AbCircle circle125;
extern const AbCircle circle126;
extern const AbCircle circle127;
extern const AbCircle circle128;
extern const AbCircle circle129;
extern const AbCircle circle130;
extern const AbCircle circle131;
extern const AbCircle circle132;
extern const AbCircle circle133;
extern const AbCircle circle134;
extern const AbCircle circle135;
extern const AbCircle circle136;
extern const AbCircle circle137;
extern const AbCircle circle138;
extern const AbCircle circle139;
extern const AbCircle circle140;
extern const AbCircle circle141;
extern const AbCircle circle142;
extern const AbCircle circle143;
extern const AbCircle circle144;
extern const AbCircle circle145;
extern const AbCircle circle146;
extern const AbCircle circle147;
extern const AbCircle circle148;
extern const AbCircle circle149;
extern const AbCircle circle150;

#endif // included 
