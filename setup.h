/*
  basically the "common.h" for this lab
*/

#ifndef _SETUP_H_
#define _SETUP_H_

#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4_discovery_lis302dl.h"
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

void delay(int delay);
int strtoint_n(char* str, int n);
int strtoint(char* str);

#endif
