# micro-p-project
McGill University ECSE 426 - final project

I worked on this project on STM32F407 microprocessor board with my partner Daniel JuneHee Lee.

This project puts together sampling techniques, filtering, and device drivers developed throughout the course
to create a two-STM32F407 microprocessor board system where transmitter board may be freely moved and the 
receiver board will mimic transmitter's movements by receiving accelerometer values through wireless communication.

The accelerometer values are calibrated based on default accelerometer faults, filtered by moving average filter,
and are exchanged using the Texas Instruments CC2500 wireless transceiver.

The driver for Texas Instruments CC2500 is written by us.

The values are also printed on Hitachi HD44780 LCD module, for which the driver is also written by us.

main.c is located on either folders transmittermain or receivermain, depending on which role the board
which will be installed onto must act.
