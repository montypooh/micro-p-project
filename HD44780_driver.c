#include "HD44780_driver.h"

void reset_all_pin()
{
    int i;
    uint16_t val = 0x0008;
  
    for(i = 0; i < 13; i++){
        GPIO_SetBits(GPIOE,val);
        val <<= 1;
    }
}

// data = '0000000000'; D7 - D0 RW RS : 10 bits
void pin_set(int data)
{
    int n;
    int number_of_set = 0;
    uint16_t val = 0x0020; // Pin 5 = RS
	
    int* pin_set;
    int pin_set_array[10];
    pin_set = pin_set_array;
	
    /*set corresponding pins*/
    for(n = 0; n < 10; n++){
        if(n == 2) {
            val <<= 1;
        }
        if(data & (1 << n)) {
            GPIO_SetBits(GPIOE,val); // check bit from n th position 
            *pin_set = n; // store pin number that is set
            pin_set++;
            number_of_set++;
        } else {
            GPIO_ResetBits(GPIOE,val);
        }
        if(n!=9) {
            val <<= 1;
        }
    }
	
    /*enable*/ 
    GPIO_SetBits(GPIOE,GPIO_Pin_7); // set high
    delay(CMDW_DELAY); // delay
    GPIO_ResetBits(GPIOE,GPIO_Pin_7); // set low
  
    /*wait for delay*/
    if (data == CLEAR_DISPLAY) {
        delay(CD_DELAY);
    } else {
        delay(CMDW_DELAY); // delay for carrying out the command
    }
	
    /*reset pins that are being set*/
    val = 0x0020;
    n = 0;
    for(; number_of_set > 0; number_of_set--) {
        pin_set--; // to pointer cursor back
    }
	
    for(n = 0; n < 10; n++){
        if(n == 2) {
            val <<= 1;
        }
        if(*pin_set == n) {
            GPIO_ResetBits(GPIOE,val); 
        }
        val <<= 1;
    }
}

void init_LCD(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); // enable connection between GPIOE and AHB1 bus
    GPIO_InitTypeDef gpio_init_disp; // make gpio initialization struct

    // set struct parameters
    gpio_init_disp.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    gpio_init_disp.GPIO_Mode = GPIO_Mode_OUT;
    gpio_init_disp.GPIO_Speed =  GPIO_Speed_100MHz;
    gpio_init_disp.GPIO_OType = GPIO_OType_PP;
    gpio_init_disp.GPIO_PuPd = GPIO_PuPd_DOWN; 

    GPIO_Init(GPIOE,&gpio_init_disp); // initialize
    reset_all_pin();
	
    pin_set(CLEAR_DISPLAY);
    pin_set(CURSOR_HOME);
    pin_set(CHARACTER_ENTRY_MODE);
    pin_set(DISPLAY_ON_OFF_CURSOR);
    pin_set(FUNCTION_SET);
}

void write_txt(const char* txt, ...)
{
    int i,j,k,l; // iterators for this method

    reset_all_pin();
    pin_set(CLEAR_DISPLAY);
    pin_set(CURSOR_HOME);
	
    va_list args; // va_list to store variable # of arguments
    va_start(args, txt); // fill va_list with arguments after txt
    
    char* container; // pointer to string after replacing %x with respective argument
    char container_dst[MAX_PRINT_LENGTH]; // allocate memory for the string
	
    char* buf; // pointer to string representation of argument
    char buf_dst[MAX_VAR_LENGTH]; // allocate memory for the string
	
    container = container_dst; // point them
    buf = buf_dst;
	
    for(i=0;*txt!='\0';txt++) { // fill container with string literal replacing %x with respective arguments
        if(*txt=='%') { // detection of replacement point
            txt++;
            switch(*txt) { // determine data-type
            case 'f': // floating point
                sprintf(buf_dst, "%f", va_arg(args, float)); // convert float variable to string, put in buf_dst
                for(;*buf!='\0';buf++) { // fill container with characters in buf_dst
                    *container = *buf;
                    container++;
                    j++; // this is the number of characters filled from replaced variables
                }
                for(l=0;l<j;l++) buf--; // undo the increment of pointer to re-use buf_dst
                txt++; // point to next character in the original message
                break;
            case 'd': // integer
                break;
            default:
                break;
            }
        }
        *container = *txt; // fill the container with next character of original message
        container++; // increment container pointer
        i++; // this is the number of characters filled from the original message
    }
    *container = '\0'; // mark the end of constructed message
    for (k=0;k<(i+j);k++) {
        container--; // decrement the pointer to beginning character of message
    }
    for(i=0;*container!='\0';container++) {
        if (i==16) for (int j=0;j<24;j++) pin_set(DC_SHIFT); // increment 24 invisible digits
        switch(*container) {
        case '0': //00110000
            pin_set(ZERO);
            break;
        case '1': //00110001
            pin_set(ONE);
            break;
        case '2': //00110010
            pin_set(TWO);
            break;
        case '3': //00110011
            pin_set(THREE);
            break;
        case '4': //00110100
            pin_set(FOUR);
            break;
        case '5': //00110101
            pin_set(FIVE);
            break;
        case '6': //00110110
            pin_set(SIX);
            break;
        case '7': //00110111
            pin_set(SEVEN);
            break;
        case '8': //00111000
            pin_set(EIGHT);
            break;
        case '9': //00111001
            pin_set(NINE);
            break;
        case 'a': //01100001 01 = 0x185   
            pin_set(LOWER_LETTER_a);
            break;
        case 'b': //01100010 01 = 0x189
            pin_set(LOWER_LETTER_b); 
            break;
        case 'c': //01100011 01 = 0x18D
            pin_set(LOWER_LETTER_c);
            break;
        case 'd': //01100100 01 = 0x191
            pin_set(LOWER_LETTER_d);
            break;
        case 'e': //01100101 01 = 0x195
            pin_set(LOWER_LETTER_e);
            break;
        case 'f': //01100110 01 = 0x199
            pin_set(LOWER_LETTER_f);
            break;
        case 'g': //01100111 01 = 0x19D
            pin_set(LOWER_LETTER_g);
            break;
        case 'h': //01101000 01 = 0x1A1
            pin_set(LOWER_LETTER_h);
            break;
        case 'i': //01101001 01 = 0x1A5
            pin_set(LOWER_LETTER_i);
            break;
        case 'j': //01101010 01 = 0x1A9
            pin_set(LOWER_LETTER_j);
            break;
        case 'k': //01101011 01 = 0x1AD
            pin_set(LOWER_LETTER_k);
            break;
        case 'l': //01101100 01 = 0x1B1
            pin_set(LOWER_LETTER_l);
            break;
        case 'm': //01101101 01 = 0x1B5
            pin_set(LOWER_LETTER_m);
            break;
        case 'n': //01101110 01 = 0x1B9
            pin_set(LOWER_LETTER_n);
            break;
        case 'o': //01101111 01 = 0x1BD
            pin_set(LOWER_LETTER_o);
            break;
        case 'p': //01110000 01 = 0x1C1
            pin_set(LOWER_LETTER_p);
            break;
        case 'q': //01110001 01 = 0x1C5
            pin_set(LOWER_LETTER_q);
            break;
        case 'r': //01110010 01 = 0x1C9
            pin_set(LOWER_LETTER_r);
            break;
        case 's': //01110011 01 = 0x1CD
            pin_set(LOWER_LETTER_s);
            break;
        case 't': //01110100 01 = 0x1D1
            pin_set(LOWER_LETTER_t);
            break;
        case 'u': //01110101 01 = 0x1D5
            pin_set(LOWER_LETTER_u);
            break;
        case 'v': //01110110 01 = 0x1D9
            pin_set(LOWER_LETTER_v);
            break;
        case 'w': //01110111 01 = 0x1DD
            pin_set(LOWER_LETTER_w);
            break;
        case 'x': //01111000 01 = 0x1E1
            pin_set(LOWER_LETTER_x);
            break;
        case 'y': //01111001 01 = 0x1E5
            pin_set(LOWER_LETTER_y);
            break;	
        case 'z': //01111010 01 = 0x1E9
            pin_set(LOWER_LETTER_z);
            break;
        case ' ': //00100000 01 = 0x081
            pin_set(SPACE);
            break;
        case ':': //00111010 01 = 0x0E9
            pin_set(COLON);
            break;
        case '=': //00111101 01 = 0x0F5
            pin_set(EQUAL);
            break;
        case '.': //00101110 01 = 0x0B9
            pin_set(PERIOD);
            break;
        case ',': //00101100 01 = 0x0B1
            pin_set(COMMA);
            break;
        case '-': //00101101 01 = 0x0B5
            pin_set(DASH);
            break;
        default:
            break;
        }
        i++;
    }
}
