//
// Created by joey on 2024-07-03.
//

#include "bma400.h"

/****************************/
/******* write & read *******/
/****************************/

static volatile bool m_xfer_done = false;

void writeReadBMA400(uint8_t *w_data, uint32_t w_length, uint8_t *r_data, uint32_t r_length)
{
		writeReadI2C(BMA400_ADDR, w_data, w_length, r_data, r_length);
}

void writeBMA400(uint8_t *w_data, uint32_t w_length)
{
		writeI2C(BMA400_ADDR, w_data, w_length);
}

/****************************/
/********** config **********/
/****************************/

void setSoftResetBMA400()
{
		uint8_t config[2];
		config[0] = BMA400_CommandReg_ADDR;
		config[1] = (0xb6);
		
		writeBMA400(&config[0], sizeof(config));
}

void setAccConfig0()
{
// filt1_bw 		   = 0x00 
// osr_lp 	 			 = 0x00
// power_mode_conf = 0x02
	
		uint8_t config[2];
		config[0] = BMA400_AccConfig0_ADDR;
		config[1] = (0x02);

		writeBMA400(&config[0], sizeof(config));
}

void setAccConfig1()
{
// acc_range  = 0x01 
// osr 				= 0x01
// acc_odr  	= 0x07
	
		uint8_t config[2];
		config[0] = BMA400_AccConfig1_ADDR;
		config[1] = (0x57);

		writeBMA400(&config[0], sizeof(config));
}

void setIntConfig0()
{
		uint8_t config[2];
		config[0] = BMA400_IntConfig0_ADDR;
		config[1] = (0x80);

		writeBMA400(&config[0], sizeof(config));
}

void setIntConfig1()
{
		uint8_t config[2];
		config[0] = BMA400_IntConfig1_ADDR;
		config[1] = (0x0C);

		writeBMA400(&config[0], sizeof(config));
}

void setInt1Map()
{
		uint8_t config[2];
		config[0] = BMA400_Int1Map_ADDR;
		config[1] = (0x80);

		writeBMA400(&config[0], sizeof(config));
}

void setInt12Map()
{
		uint8_t config[2];
		config[0] = BMA400_Int12Map_ADDR;
		config[1] = (0x40);

		writeBMA400(&config[0], sizeof(config));
}

void setTapConfig0()
{
		uint8_t config[2];
		config[0] = BMA400_TAPConfig0_ADDR;
		config[1] = (0x00);

		writeBMA400(&config[0], sizeof(config));
}

void setTapConfig1()
{
		uint8_t config[2];
		config[0] = BMA400_TAPConfig1_ADDR;
		config[1] = (0x00);

		writeBMA400(&config[0], sizeof(config));
}

/****************************/
/********* printer **********/
/****************************/

void printerConectCheckBMA400()
{
    ret_code_t err_code;

    uint8_t rdData;
		uint8_t wrData[1] = {BMA400_ChipID_ADDR}; 
		
		err_code = writeReadI2C(BMA400_ADDR, wrData, sizeof(wrData), &rdData, sizeof(rdData));
    APP_ERROR_CHECK(err_code);

    if (err_code == BMA400_ChipID_ADDR)
    {
			NRF_LOG_INFO("BMA400 init Success");
    }
    else
    {
    	NRF_LOG_INFO("BMA400 init Fail");
    }
}

/****************************/
/******** data read *********/
/****************************/

static uint8_t accXYZRaw[6];

void readAccXYZ()
{
		uint8_t wrData[1] = {BMA400_AccXLSB_ADDR};
	
    writeReadBMA400(wrData, sizeof(wrData), &accXYZRaw[0], sizeof(accXYZRaw));
}

/****************************/
/****** data converter ******/
/****************************/

static int16_t accX = 0;
static int16_t accY = 0;
static int16_t accZ = 0;

void convAccXYZ(void)
{
    accX = (int16_t)(((uint16_t)accXYZRaw[1] << 8) | accXYZRaw[0]);
    accY = (int16_t)(((uint16_t)accXYZRaw[3] << 8) | accXYZRaw[2]);
    accZ = (int16_t)(((uint16_t)accXYZRaw[5] << 8) | accXYZRaw[4]);

    if(accX > 2047) accX = (accX - 4096);
    if(accY > 2047) accY = (accY - 4096);
    if(accZ > 2047) accZ = (accZ - 4096);
}

/****************************/
/********** bma400 **********/
/****************************/

void initBMA400()
{
    setSoftResetBMA400();
    nrf_delay_ms(2);
    setAccConfig0();
    nrf_delay_us(200);
    setAccConfig1();
    nrf_delay_us(200);
    setIntConfig0();
    nrf_delay_us(200);
    setInt1Map();
    nrf_delay_ms(2);
	
		printerConectCheckBMA400();
}

void initBMA400Tap()
{
    setSoftResetBMA400();
    nrf_delay_ms(2);
		setIntConfig1();
    nrf_delay_us(200);
		setInt12Map();
    nrf_delay_us(200);
		setTapConfig0();
    nrf_delay_us(200);
		setTapConfig1();
    nrf_delay_us(200);
		
		printerConectCheckBMA400();
}

void BMA400()
{
    readAccXYZ();
    convAccXYZ();
		
		NRF_LOG_INFO("X:%d Y:%d Z:%d", accX, accY, accZ);
    NRF_LOG_FLUSH();
		
//    saveBMA400(accX, accY, accZ);
}
