#include "mode_switch.h"
#include "setup.h"

void init_mode_switch() {
	
    // GPIOE configuration to use PA0 for 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); // Enable APB2
	
    GPIO_InitTypeDef gpio_init_mode;
    gpio_init_mode.GPIO_Pin = GPIO_Pin_0;
    gpio_init_mode.GPIO_Mode = GPIO_Mode_IN; // Input configuration 
    gpio_init_mode.GPIO_Speed = GPIO_Speed_100MHz; 
    gpio_init_mode.GPIO_OType = GPIO_OType_PP; //push-pull
    gpio_init_mode.GPIO_PuPd = GPIO_PuPd_DOWN; 
    GPIO_Init(GPIOA, &gpio_init_mode);
	
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0); // Connect EXTI to PA0
	
    // EXTI configuration
    EXTI_InitTypeDef exti_itd_mode;
    exti_itd_mode.EXTI_Line = EXTI_Line0; // external interrupt line 0
    exti_itd_mode.EXTI_Mode = EXTI_Mode_Interrupt; // interrupt mode
    exti_itd_mode.EXTI_Trigger = EXTI_Trigger_Rising; // rising edge
    exti_itd_mode.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti_itd_mode);

    //NVIC configuration: mapping to EXTI line
    NVIC_InitTypeDef nvic_itd_mode;
    nvic_itd_mode.NVIC_IRQChannel = EXTI0_IRQn; // mapping to EXTI 1
    nvic_itd_mode.NVIC_IRQChannelPreemptionPriority = 0; // interrupt priority group 1
    nvic_itd_mode.NVIC_IRQChannelSubPriority = 0; // priority among interrupts are the highest
    nvic_itd_mode.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_itd_mode);

}
