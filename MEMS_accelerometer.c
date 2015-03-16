#include "setup.h"

void init_mems(void)
{
    //MEMS sensor configuration
    LIS302DL_InitTypeDef lis302dl_itd;
    lis302dl_itd.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE; //Low power mode
    lis302dl_itd.Output_DataRate = LIS302DL_DATARATE_100; // 100 Hz sampling
    lis302dl_itd.Axes_Enable = LIS302DL_X_ENABLE|LIS302DL_Y_ENABLE|LIS302DL_Z_ENABLE;
    lis302dl_itd.Full_Scale = LIS302DL_FULLSCALE_2_3;
    lis302dl_itd.Self_Test = LIS302DL_SELFTEST_NORMAL;
    LIS302DL_Init(&lis302dl_itd);

    // MEMS sensor interrupt configuration
    LIS302DL_InterruptConfigTypeDef  lis302dl_ictd;
    lis302dl_ictd.Latch_Request = LIS302DL_INTERRUPTREQUEST_NOTLATCHED; // non-latched
    lis302dl_ictd.SingleClick_Axes = LIS302DL_CLICKINTERRUPT_XYZ_DISABLE;
    lis302dl_ictd.DoubleClick_Axes = LIS302DL_DOUBLECLICKINTERRUPT_XYZ_DISABLE;
    LIS302DL_InterruptConfig(&lis302dl_ictd);

    uint8_t pBuffer = 0x20; // 7: active high,	 6: push-pull 
                            //[5:3] = 100 for INT2 'data ready'

    // write 00100000 to address of control register 3
    LIS302DL_Write(&pBuffer, LIS302DL_CTRL_REG3_ADDR, 1);

    // GPIOE configuration to use PE1 for INT2 of MEMS sensor
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); // Enable APB2

    GPIO_InitTypeDef gpio_init_mems;
    gpio_init_mems.GPIO_Pin = GPIO_Pin_1;
    gpio_init_mems.GPIO_Mode = GPIO_Mode_IN; // Input configuration 
    gpio_init_mems.GPIO_Speed = GPIO_Speed_100MHz; 
    gpio_init_mems.GPIO_OType = GPIO_OType_PP; //push-pull
    gpio_init_mems.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOE, &gpio_init_mems);
    
    // choose input pin for EXTI line
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1); // Connect EXTI to PE0

    // EXTI configuration
    EXTI_InitTypeDef EXTI_itd_mems;
    EXTI_itd_mems.EXTI_Line = EXTI_Line1; // external interrupt line 1
    EXTI_itd_mems.EXTI_Mode = EXTI_Mode_Interrupt; // interrupt mode
    EXTI_itd_mems.EXTI_Trigger = EXTI_Trigger_Rising; // rising edge
    EXTI_itd_mems.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_itd_mems);

    //NVIC configuration: mapping to EXTI line
    NVIC_InitTypeDef NVIC_itd_mems;
    NVIC_itd_mems.NVIC_IRQChannel = EXTI1_IRQn; // mapping to EXTI 1
    NVIC_itd_mems.NVIC_IRQChannelPreemptionPriority = 0; // interrupt priority group 1
    NVIC_itd_mems.NVIC_IRQChannelSubPriority = 1; // highest priority among interrupts
    NVIC_itd_mems.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_itd_mems);
}
