#include "arm_math.h" /*base project includes*/
#include "stm32f4xx.h"
#include "cmsis_os.h"

#include "servo_motor.h" /*angle output and wireless receive includes*/
#include "CC2500_driver.h"

#include "MA_filter.h" /*angle detector + thermometer includes*/
#include "HD44780_driver.h"

/* prototype accelerometer & temperature sampling, lcd display threads */
void t_angle(void const* argument);
void t_output(void const* argument);

osThreadDef(t_angle, osPriorityHigh, 1, 0);
osThreadDef(t_output, osPriorityNormal, 1, 0);

float pitch, roll; // pitch and roll of board w.r.t x,y axis
filter_t fpitch, froll;
uint8_t status, read[3];

osThreadId tid_angle;
osThreadId tid_output;
 
int main (void)
{
    init_LCD(); // initialize LCD
    CC2500_Init(); // initialize wireless communication
    init_servo(); // initialize motor pwm
  
    /* Start threads */
    tid_angle = osThreadCreate(osThread(t_angle), NULL);
    tid_output = osThreadCreate(osThread(t_output), NULL);
	
    osDelay(osWaitForever); // main thread lives forever
}

void t_angle (void const* argument)
{
    /* struct initialization for moving average filter */
    init_filter(&fpitch);
    init_filter(&froll);
	
    CC2500_Read(&status, MARCSTATE, BURSTREGREAD);
	
    while(1) {
        CC2500_Read(&status, _SFRX, COMMANDSTROBE);
        CC2500_Read(&status, _SRX, COMMANDSTROBE);
        while(status!=0x01) {
            CC2500_Read(&status, MARCSTATE, BURSTREGREAD);    
        }
        CC2500_Read(read, RXFIFO, 3);

        filter(&fpitch, (float)read[1]); /* filter pitch and roll */
        filter(&froll, (float)read[2]);

        pitch = fpitch.average; /* calculate pitch and roll */
        roll = froll.average;
        osDelay(1);
    }
}

void t_output(void const* argument)
{
    while(1) {
        //write_txt("pitch = %f roll = %f", pitch, roll);
        servo(roll,pitch);
        osDelay(1);
    }
}
