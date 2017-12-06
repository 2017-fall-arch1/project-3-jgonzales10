#ifndef buzzer_included
#define buzzer_included

void buzzer_init();
void buzzer_noise1(int volume);
void buzzer_noise2(int volume);
void buzzer_noise3();
void buzzer_noise4();
void buzzer_noiseNull();
void buzzer_set_period(short cycles, int softness);

#endif // included
