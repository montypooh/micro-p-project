/*
  driver for texas instrument cc2500 wireless transceiver

  PIN CONFIGURATIONS
  on the board:   on the chip:
  pb12: ss        p17: ss
  pb13: sck       p16: sck
  pb14: miso      p15: miso
  pb15: mosi      p18: mosi
*/

#ifndef _CC2500_DRIVER_H_
#define _CC2500_DRIVER_H_

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"

void CC2500_Init(void);

static void CC2500_SendByte(uint8_t *dst, uint8_t src);
void CC2500_Write(uint8_t* dst, uint8_t* src, uint8_t addr, uint16_t NumByteToWrite);
void CC2500_Read(uint8_t* dst, uint8_t addr, uint16_t NumByteToRead);

#define FIFOREAD 1 /* modes of operation */
#define COMMANDSTROBE 0
#define CHANGECONFIG 1
#define BURSTREGREAD 1 

#define CC2500_FLAG_TIMEOUT   ((uint32_t)0x1000)

#ifndef SMARTRF_CC2500_H 
#define SMARTRF_CC2500_H 
 
#define SMARTRF_RADIO_CC2500 
 
#define SMARTRF_SETTING_FSCTRL1 0x0C//0x12 //Frequency offset = 457kHz 
#define SMARTRF_SETTING_FSCTRL0 0x00 
#define SMARTRF_SETTING_FREQ2 0x5D // Carrier Frequency is 2.433GHz 
#define SMARTRF_SETTING_FREQ1 0x93 
#define SMARTRF_SETTING_FREQ0 0xB1 
#define SMARTRF_SETTING_MDMCFG4 0x0E //0x2D // BW of channel = 541.666kHz 
#define SMARTRF_SETTING_MDMCFG3 0x3B // Baud Rate = 125kb 
#define SMARTRF_SETTING_MDMCFG2 0x73 //before demodulator, MSK modulation, 16/16 sync word bits detected
#define SMARTRF_SETTING_MDMCFG1 0xC2
#define SMARTRF_SETTING_MDMCFG0 0xF8 // Default Channel Spacing of 200kHz 
#define SMARTRF_SETTING_CHANNR 0x00 // Channel 0 
#define SMARTRF_SETTING_DEVIATN 0x00 //0x01 // 1785kHz 
#define SMARTRF_SETTING_FREND1 0xB6 
#define SMARTRF_SETTING_FREND0 0x10 
#define SMARTRF_SETTING_MCSM1 0x00 // when transmission or reception done go idle
#define SMARTRF_SETTING_MCSM0 0x18 // Automatically calibrate When going from IDLE to RX or TX (or FSTXON) check CC2500 datasheet
#define SMARTRF_SETTING_FOCCFG 0x1D // check datasheet 
#define SMARTRF_SETTING_BSCFG 0x1C 
#define SMARTRF_SETTING_AGCCTRL2 0xC7 
#define SMARTRF_SETTING_AGCCTRL1 0x40 //0x00 
#define SMARTRF_SETTING_AGCCTRL0 0xB0 
#define SMARTRF_SETTING_FSCAL3 0xEA 
#define SMARTRF_SETTING_FSCAL2 0x0A 
#define SMARTRF_SETTING_FSCAL1 0x00 
#define SMARTRF_SETTING_FSCAL0 0x19 //0x11 
#define SMARTRF_SETTING_FSTEST 0x59 
#define SMARTRF_SETTING_TEST2 0x88 
#define SMARTRF_SETTING_TEST1 0x31 
#define SMARTRF_SETTING_TEST0 0x0B 
#define SMARTRF_SETTING_FIFOTHR 0x00
#define SMARTRF_SETTING_IOCFG2 0x29 
#define SMARTRF_SETTING_IOCFG0 0x06 
#define SMARTRF_SETTING_PKTCTRL1 0x09 // crc autoflush if crc bad, enable address filtration
#define SMARTRF_SETTING_PKTCTRL0 0x04 // fixed packet length, crc check enabled
#define SMARTRF_SETTING_ADDR 0x87 // custom address 
#define SMARTRF_SETTING_PKTLEN 0x03 // set packet length 3: 1 address byte, 2 data bytes
 
#endif

/* configuration register addresses */
#define FSCTRL1   0x0b
#define FSCTRL0   0x0c
#define FREQ2     0x0d
#define FREQ1     0x0e
#define FREQ0     0x0f
#define MDMCFG4   0x10
#define MDMCFG3   0x11
#define MDMCFG2   0x12
#define MDMCFG1   0x13
#define MDMCFG0   0x14
#define CHANNR    0x0a
#define DEVIATN   0x15
#define FREND1    0x21
#define FREND0    0x22
#define MCSM1     0x17
#define MCSM0     0x18
#define FOCCFG    0x19
#define BSCFG     0x1a
#define AGCCTRL2  0x1b
#define AGCCTRL1  0x1c
#define AGCCTRL0  0x1d
#define FSCAL3    0x23
#define FSCAL2    0x24
#define FSCAL1    0x25
#define FSCAL0    0x26
#define FSTEST    0x29
#define TEST2     0x2c
#define TEST1     0x2d
#define TEST0     0x2e
#define FIFOTHR   0x03
#define IOCFG2    0x00
#define IOCFG0    0x02
#define PKTCTRL1  0x07
#define PKTCTRL0  0x08
#define ADDR      0x09
#define PKTLEN    0x06
#define SYNC1     0x04
#define SYNC0     0x05

/* command strobe addresses */
#define _SRES      0x30
#define _SRX       0x34
#define _STX       0x35
#define _SIDLE     0x36
#define _SFRX      0x3a
#define _SFTX      0x3b
#define _SNOP      0x3d

/* fifo addresses depending on mode */
#define TXFIFO  0x3f
#define RXFIFO  0x3f

/* burst registers */
#define TXBYTES     0xfa
#define RXBYTES     0xfb
#define MARCSTATE   0xf5

#endif
