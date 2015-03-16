#include "servo_motor.h"

const int scale1 = 35;
const int scale2 = 36;

uint16_t CCR1_Val,CCR2_Val;

void init_servo(void)
{
	
    GPIO_InitTypeDef gpio_init_pwm;
  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); /* TIM3 clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); /* GPIOC clock enable */
    
    /* GPIOD Configuration: TIM3 CH1 (PC6) TIM3 CH4 (PC9) */
    gpio_init_pwm.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_9;
    gpio_init_pwm.GPIO_Mode = GPIO_Mode_AF;
    gpio_init_pwm.GPIO_Speed = GPIO_Speed_100MHz;
    gpio_init_pwm.GPIO_OType = GPIO_OType_PP;
    gpio_init_pwm.GPIO_PuPd = GPIO_PuPd_UP;
    
    GPIO_Init(GPIOC, &gpio_init_pwm); 
  
    /* Connect TIM3pins to AF1 */  
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
    
    TIM_TimeBaseInitTypeDef  tim_tbs_pwm;

    // counter clock 32768000 Hz = (ARR+1) * 50 Hz
    uint16_t PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 3276800) - 1;

    /* Time base configuration */
    tim_tbs_pwm.TIM_Period = 65535; // ARR = 65535 to have the finest roll resolution 
    tim_tbs_pwm.TIM_Prescaler = PrescalerValue; // Prescaler = (Systemcoreclock/2)/3276800-1
    tim_tbs_pwm.TIM_ClockDivision = 0;
    tim_tbs_pwm.TIM_CounterMode = TIM_CounterMode_Up; // Up counter mode
    
    TIM_TimeBaseInit(TIM3, &tim_tbs_pwm);	

    TIM_OCInitTypeDef tim_ocis_pwm;

    /* PWM1 Mode configuration: Channel 1 */
    tim_ocis_pwm.TIM_OCMode = TIM_OCMode_PWM1; // PWM output compare mode
    tim_ocis_pwm.TIM_OutputState = TIM_OutputState_Enable;
    tim_ocis_pwm.TIM_Pulse = 0 ; // determines duty cycle
    tim_ocis_pwm.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM3, &tim_ocis_pwm);
    TIM_OC4Init(TIM3, &tim_ocis_pwm);
	
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);	 
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);	
    
    TIM_ARRPreloadConfig(TIM3, ENABLE);

    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE);
}

void servo(float roll, float pitch)
{
    //CCR1_Val = 2000 + scale*(90 + roll); // pulse high from (36.1 * roll) clock count
    // 2000 = -90 degree
    if(-90.1<roll&&roll<0.1) { // -90 to 0 degree
        CCR1_Val = 2000 + scale1*(90+roll);
    } else if(0.1<roll&&roll<90.1) { // 0 to 90 degree
        CCR1_Val = 5150 + scale2*roll;
    }

    if(-90.1<pitch&&pitch<0.1) { // -90 to 0 degree
        CCR2_Val = 2000 + scale1*(90+pitch);
    } else if(0.1<pitch&&pitch<90.1) { // 0 to 90 degree
        CCR2_Val = 5150 + scale2*pitch;
    }
    
    TIM_SetCompare1(TIM3,CCR1_Val);
    TIM_SetCompare4(TIM3,CCR2_Val);
}
