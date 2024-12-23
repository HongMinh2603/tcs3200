#ifndef TCS3200_H
#define TCS3200_H

#include "driver/gpio.h"
#include "driver/pcnt.h"
#define TOLERANCE 5 

#define S0 GPIO_NUM_22
#define S1 GPIO_NUM_23
#define S2 GPIO_NUM_2
#define S3 GPIO_NUM_4
#define sensorOut GPIO_NUM_15
#define PCNT_UNIT PCNT_UNIT_0

void setup_pins();
void setup_pcnt();
int get_frequency(int s2_level, int s3_level);
int inRange(int value, int target, int tolerance);
const char* getColorName(int R, int G, int B);

#endif // TCS3200_H
