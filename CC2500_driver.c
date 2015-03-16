#include "CC2500_driver.h"

static void CC2500_LowLevel_Init(void);
static void CC2500_SendByte(uint8_t *dst, uint8_t src);

__IO uint32_t  CC2500TimeOut = CC2500_FLAG_TIMEOUT;

static uint8_t CC2500_CSn_Low()
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==(uint8_t)Bit_SET);
    return 1;
}

static uint8_t CC2500_CSn_High()
{
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
    return 1;
}

void CC2500_Read(uint8_t* dst, uint8_t addr, uint16_t NumByteToRead)
{
    uint8_t dummy = 0x00; // dummy byte
    addr |= 0x80; // set read bit
    if(NumByteToRead > 0x01) {
        addr |= 0x40;   // set burst bit
    }
    CC2500_CSn_Low(); // pull CSn low
    CC2500_SendByte(dst, addr); // write address header
    while(NumByteToRead--) {
        CC2500_SendByte(dst, dummy); // send dummy byte to clock the signal
        dst++;
    }
    CC2500_CSn_High(); /* end of transaction, pull back CSn high */
}

void CC2500_Write(uint8_t* dst, uint8_t* src, uint8_t addr, uint16_t NumByteToWrite)
{
    if(NumByteToWrite > 0x01) {// if burst mode, set B on address header
        addr |= (uint8_t)0x40; // 0x40 = 0100 0000 on the address header
    }
    CC2500_CSn_Low();
    CC2500_SendByte(dst, addr); // send address, chip status register goes in dst
    while(NumByteToWrite--) { /* keep writing bytes until no more bytes to write */
        CC2500_SendByte(dst, *src);
        src++;
    }
    CC2500_CSn_High();
}

static void CC2500_SendByte(uint8_t *dst, uint8_t src)
{
    CC2500TimeOut = CC2500_FLAG_TIMEOUT;
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) {
        if((CC2500TimeOut--) == 0) {
            GPIO_SetBits(GPIOA, GPIO_Pin_9);
        }
    }
    SPI_I2S_SendData(SPI2, src);
    CC2500TimeOut = CC2500_FLAG_TIMEOUT;
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) { //stay here when it's empty
        if((CC2500TimeOut--) == 0) {
            GPIO_SetBits(GPIOA, GPIO_Pin_9);
        }
    }
    *dst = (uint8_t)SPI_I2S_ReceiveData(SPI2);
}

static void CC2500_LowLevel_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    /* enable spi2 clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    /* enable ss, sck, mosi, miso clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /* map alternate functions onto SCK, SS, MISO, MOSI */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2); // mapping sck AF
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2); // mapping miso AF
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2); // mapping mosi AF

    /* configurations for sck, miso, mosi pins */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /* configure sck pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* configure miso pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* configure mosi pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    SPI_I2S_DeInit(SPI2);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2, ENABLE);

    /* configure sck pin */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOB, GPIO_Pin_12); // set CSn
}

void CC2500_WirelessConfig()
{
    uint8_t status, ctrl;
    uint8_t dummy = 0x00; // dummy byte
    
    CC2500_Write(&status, &dummy, _SRES, 1); // do a system reset
    
    ctrl = SMARTRF_SETTING_FSCTRL1;
    CC2500_Write(&status, &ctrl, FSCTRL1, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_FSCTRL0;
    CC2500_Write(&status, &ctrl, FSCTRL0, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_FREQ2;
    CC2500_Write(&status, &ctrl, FREQ2, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_FREQ1;
    CC2500_Write(&status, &ctrl, FREQ1, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_FREQ0;
    CC2500_Write(&status, &ctrl, FREQ0, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_MDMCFG4;
    CC2500_Write(&status, &ctrl, MDMCFG4, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_MDMCFG3;
    CC2500_Write(&status, &ctrl, MDMCFG3, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_MDMCFG2;
    CC2500_Write(&status, &ctrl, MDMCFG2, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_MDMCFG1;
    CC2500_Write(&status, &ctrl, MDMCFG1, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_MDMCFG0;
    CC2500_Write(&status, &ctrl, MDMCFG0, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_CHANNR;
    CC2500_Write(&status, &ctrl, CHANNR, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_DEVIATN;
    CC2500_Write(&status, &ctrl, DEVIATN, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_FREND1;
    CC2500_Write(&status, &ctrl, FREND1, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_FREND0;
    CC2500_Write(&status, &ctrl, FREND0, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_MCSM1;
    CC2500_Write(&status, &ctrl, MCSM1, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_MCSM0;
    CC2500_Write(&status, &ctrl, MCSM0, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_FOCCFG;
    CC2500_Write(&status, &ctrl, FOCCFG, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_BSCFG;
    CC2500_Write(&status, &ctrl, BSCFG, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_AGCCTRL2;
    CC2500_Write(&status, &ctrl, AGCCTRL2, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_AGCCTRL1;
    CC2500_Write(&status, &ctrl, AGCCTRL1, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_AGCCTRL0;
    CC2500_Write(&status, &ctrl, AGCCTRL0, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_FSCAL3;
    CC2500_Write(&status, &ctrl, FSCAL3, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_FSCAL2;
    CC2500_Write(&status, &ctrl, FSCAL2, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_FSCAL1;
    CC2500_Write(&status, &ctrl, FSCAL1, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_FSCAL0;
    CC2500_Write(&status, &ctrl, FSCAL0, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_FSTEST;
    CC2500_Write(&status, &ctrl, FSTEST, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_TEST2;
    CC2500_Write(&status, &ctrl, TEST2, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_TEST1;
    CC2500_Write(&status, &ctrl, TEST1, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_TEST0;
    CC2500_Write(&status, &ctrl, TEST0, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_FIFOTHR;
    CC2500_Write(&status, &ctrl, FIFOTHR, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_IOCFG2;
    CC2500_Write(&status, &ctrl, IOCFG2, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_IOCFG0;
    CC2500_Write(&status, &ctrl, IOCFG0, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_PKTCTRL1;
    CC2500_Write(&status, &ctrl, PKTCTRL1, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_PKTCTRL0;
    CC2500_Write(&status, &ctrl, PKTCTRL0, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_ADDR;
    CC2500_Write(&status, &ctrl, ADDR, CHANGECONFIG);
    ctrl = SMARTRF_SETTING_PKTLEN;
    CC2500_Write(&status, &ctrl, PKTLEN, CHANGECONFIG);
}

void CC2500_Init()
{
    CC2500_LowLevel_Init();
    CC2500_WirelessConfig();
}
