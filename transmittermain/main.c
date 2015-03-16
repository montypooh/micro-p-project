/* base project includes */
#include "arm_math.h"
#include "stm32f4xx.h"
#include "cmsis_os.h"

/* angle processing includes */
#include "3DLSM_calibrator.h"
#include "MEMS_accelerometer.h"
#include "atan_LUT.h"
#include "MA_filter.h"

/* wireless and graphical output includes */
#include "CC2500_driver.h"
#include "HD44780_driver.h"
#include "Keypad_driver.h"

/* mode switching enable includes */
#include "mode_switch.h"

/* modes of operation */
#define MIMIC 0
#define SEQUENCE 1

/* set initial mode of operation*/
uint8_t MODE = MIMIC;

/*prototype accelerometer and temperature sampling threads*/
void t_angle(void const* argument);

/*prototype lcd display thread*/
void t_lcd(void const* argument);

/*prototype transmitter thread*/
void t_output(void const* argument);

osThreadDef(t_angle, osPriorityHigh, 1, 0); // sampling thread higher priority
osThreadDef(t_output, osPriorityNormal, 1, 0);
osThreadDef(t_lcd, osPriorityNormal, 1, 0);

float pitch, roll; // pitch and roll of board w.r.t x,y axis
uint8_t status, write[3], dummy = 0x00;

char sequence[5][3][3] = {0};
int schedule[5][3] = {0};

int i = 0; // point count
int j = 0; // pitch, roll, time count
int k = 0; // digit count
int ready = 0;

int32_t SIGNAL_ANGLE = 0x00000001;
int32_t SIGNAL_KEYPAD = 0x00000001;

osThreadId tid_angle, tid_output, tid_lcd;
 
int main (void)
{
    init_LCD();
    init_mode_switch();
    Keypad_Init();
    
    tid_angle = osThreadCreate(osThread(t_angle), NULL);
    tid_output = osThreadCreate(osThread(t_output), NULL);
    tid_lcd = osThreadCreate(osThread(t_lcd), NULL);
    
    osDelay(osWaitForever); // main thread lives forever
}

void t_angle (void const* argument)
{
    init_mems(); // initialize mems
    CC2500_Init();
    
    /* struct initialization for moving average filter */
    filter_t fx, fy, fz;
    init_filter(&fx);
    init_filter(&fy);
    init_filter(&fz);
	
    int32_t out[3]; // array of accelometer reading wrt x,y,z axis
    
    write[0] = 0x87; // write address
    
    LIS302DL_ReadACC(out); // begin interrupts
    CC2500_Read(&status, MARCSTATE, BURSTREGREAD);
    
    while(1) {
        osSignalWait(SIGNAL_ANGLE, osWaitForever);
		
        LIS302DL_ReadACC(out); // store acceleration ACC
        calibrate(out); // calibration
		
        /* filter each axis */
        filter(&fx, out[0]);
        filter(&fy, out[1]);
        filter(&fz, out[2]);

        /* calculate pitch and roll */
        pitch = atan_table(fx.average/sqrt(fy.average*fy.average+fz.average*fz.average));
        roll = atan_table(fy.average/sqrt(fx.average*fx.average+fz.average*fz.average));
    }
}

void t_output(void const* argument)
{
    char c;
    while(1) {
        if(MODE==MIMIC) {
            write_txt("pitch = %f roll = %f", pitch, roll);
            write[1] = (uint8_t)pitch;
            write[2] = (uint8_t)roll;
            CC2500_Write(&status, &dummy, _SFTX, COMMANDSTROBE);
            CC2500_Write(&status, write, TXFIFO, 3);
            CC2500_Write(&status, &dummy, _STX, COMMANDSTROBE); // begin transfer
            while(status!=0x01) { // wait until everything has been sent
                CC2500_Read(&status, MARCSTATE, BURSTREGREAD);
            }
        } else if (MODE==SEQUENCE) {
        } // code deleted due to not being complete
        osDelay(10);
    }
}

void t_lcd(void const* argument)
{
    while(1) {
        if(MODE==MIMIC) {
            write_txt("pitch = %f roll = %f", pitch, roll);
        } else if(MODE==SEQUENCE) {
            write_txt("p = %s r = %s t = %s",
                      sequence[i][0],
                      sequence[i][1],
                      sequence[i][2]);
        }
        osDelay(20);
    }
}

// MODE switch ISR
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line0);
        MODE++;
        MODE%=2;
    }
}

// MEMS sensor ISR
void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line1);
        osSignalSet(tid_angle, SIGNAL_ANGLE);
    }
}
