#include "Keypad_driver.h"

uint8_t ctrl = 0x00;
uint8_t prevctrl = 0x00;

void Keypad_Init()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

uint8_t Keypad_RowScan()
{
    if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1))
        ctrl|=0x80;
    else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2))
        ctrl|=0x40;
    else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3))
        ctrl|=0x20;
    else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6))
        ctrl|=0x10;
    else
        ctrl=0x00;
    return ctrl;
}

uint8_t Keypad_ColScan(uint8_t ctrl)
{
    if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7))
        ctrl|=0x08;
    else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8))
        ctrl|=0x04;
    else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9))
        ctrl|=0x02;
    else
        ctrl=0x00;
    return ctrl;
}

char Keypad_GetKey()
{
    ctrl = Keypad_ColScan(Keypad_RowScan());
    if(prevctrl!=ctrl) {
        prevctrl = ctrl;
        switch(ctrl) {
        case 0x88: // row 1 col 1
            return '1';
        case 0x84: // row 1 col 2
            return '2';
        case 0x82: // row 1 col 3
            return '3';
        case 0x48: // row 2 col 1
            return '4';
        case 0x44: // row 2 col 2
            return '5';
        case 0x42: // row 2 col 3
            return '6';
        case 0x28: // row 3 col 1
            return '7';
        case 0x24: // row 3 col 2
            return '8';
        case 0x22: // row 3 col 3
            return '9';
        case 0x18: // row 4 col 1
            return '*';
        case 0x14: // row 4 col 2
            return '0';
        case 0x12: // row 4 col 3
            return '#';
        default:
            return NULL;
        }
    }
    else {
        return NULL;
    }
}
