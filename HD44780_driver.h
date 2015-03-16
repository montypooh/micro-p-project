/*
  driver for hitachi HD44780 lcd module
  uses variable length arguments to work like printf();

  PIN ASSIGNMENTS:
  PE3 = L+
  PE4 = L-
  PE5 = Register Select
  PE6 = Read/Write
  PE7 = Enable
  PE8 = D0
  PE9 = D1
  PE10 = D2
  PE11 = D3
  PE12 = D4
  PE13 = D5
  PE14 = D6
  PE15 = D7
*/

#ifndef _HD44780_DRIVER_H_
#define _HD44780_DRIVER_H_

#include "setup.h"
#define CMDW_DELAY 1550
#define CD_DELAY 65000
#define MAX_PRINT_LENGTH 40
#define MAX_VAR_LENGTH 10
#define DELAY 1

#define CLEAR_DISPLAY 0x004
#define RESET_ALL_PIN 2
#define CURSOR_HOME 0x008
#define CHARACTER_ENTRY_MODE 0x018
#define DISPLAY_ON_OFF_CURSOR 0x030
#define DC_SHIFT 0x050
#define FUNCTION_SET 0x0E0

#define ZERO 0x0C1
#define ONE 0x0C5
#define TWO 0x0C9
#define THREE 0x0CD
#define FOUR 0x0D1
#define FIVE 0x0D5
#define SIX 0x0D9
#define SEVEN 0x0DD
#define EIGHT 0x0E1
#define NINE 0x0E5
#define LOWER_LETTER_a 0x185
#define LOWER_LETTER_b 0x189
#define LOWER_LETTER_c 0x18D
#define LOWER_LETTER_d 0x191
#define LOWER_LETTER_e 0x195
#define LOWER_LETTER_f 0x199
#define LOWER_LETTER_g 0x19D
#define LOWER_LETTER_h 0x1A1
#define LOWER_LETTER_i 0x1A5
#define LOWER_LETTER_j 0x1A9
#define LOWER_LETTER_k 0x1AD
#define LOWER_LETTER_l 0x1B1
#define LOWER_LETTER_m 0x1B5
#define LOWER_LETTER_n 0x1B9
#define LOWER_LETTER_o 0x1BD
#define LOWER_LETTER_p 0x1C1
#define LOWER_LETTER_q 0x1C5
#define LOWER_LETTER_r 0x1C9
#define LOWER_LETTER_s 0x1CD
#define LOWER_LETTER_t 0x1D1
#define LOWER_LETTER_u 0x1D5
#define LOWER_LETTER_v 0x1D9
#define LOWER_LETTER_w 0x1DD
#define LOWER_LETTER_x 0x1E1
#define LOWER_LETTER_y 0x1E5
#define LOWER_LETTER_z 0x1E9
#define SPACE 0x081
#define COLON 0x0E9
#define EQUAL 0x0F5
#define PERIOD 0x0B9
#define COMMA 0x0B1
#define DASH 0x0B5

void init_LCD(void);
void write_txt(const char* txt, ...);

#endif
