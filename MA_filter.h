/*
  implementation of a moving average filter
*/

#ifndef _MA_FILTER_H_
#define _MA_FILTER_H_

#include "setup.h"

#define D 20

typedef struct {
    int index;
    float buffer[D];
    float sum;
    float average;
    int filled;
} filter_t;

void init_filter(filter_t* fp);
void filter(filter_t* fp, float Vsense);

#endif
