/*
  driver for keypad used in lab
  not sure what brand
*/

#ifndef _KEYPAD_DRIVER_H_
#define _KEYPAD_DRIVER_H_

#include "setup.h"

void Keypad_Init(void);
uint8_t Keypad_RowScan(void);
uint8_t Keypad_ColScan(uint8_t ctrl);
char Keypad_GetKey(void);
void Keypad_InputReady(void);
void Keypad_InputComplete(void);
int Keypad_Ready(void);

#endif
