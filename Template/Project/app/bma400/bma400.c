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
	config[1] = (0xB6);

	writeBMA400(&config[0], sizeof(config));
}

void setAccConfig0()
{
	uint8_t config[2];
	config[0] = BMA400_AccConfig0_ADDR;
	config[1] = (0x02);

	writeBMA400(&config[0], sizeof(config));
}

void setAccConfig1()
{
	uint8_t config[2];
	config[0] = BMA400_AccConfig1_ADDR;
	config[1] = (0x57);  // 7 : 50Hz, 9 : 200Hz
    
	writeBMA400(&config[0], sizeof(config));
}

void setIntConfig0()
{
	uint8_t config[2];
	config[0] = BMA400_IntConfig0_ADDR;
	config[1] = (0x80);

	writeBMA400(&config[0], sizeof(config));
}

void setInt1Map()
{
	uint8_t config[2];
	config[0] = BMA400_Int1Map_ADDR;
	config[1] = (0x80);

	writeBMA400(&config[0], sizeof(config));
}

/****************************/
/********* Tap config *******/
/****************************/
void setAccConfig1Tap()
{
	uint8_t config[2];
	config[0] = BMA400_AccConfig1_ADDR;
	config[1] = (0x19); // 200Hz

	writeBMA400(&config[0], sizeof(config));
}

void setIntConfig1()
{
	uint8_t config[2];
	config[0] = BMA400_IntConfig1_ADDR;
	config[1] = (0x04);

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
	config[1] = (0x00); //0x03

	writeBMA400(&config[0], sizeof(config));
}

void setTapConfig1()
{
	uint8_t config[2];
	config[0] = BMA400_TAPConfig1_ADDR;
	config[1] = (0x03);	//0x06

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

static uint8_t tapIntStat[1];
void readTapDetection()
{
	uint8_t wrData[1] = {BMA400_IntStat1_ADDR};

	writeReadBMA400(wrData, sizeof(wrData), &tapIntStat[0], sizeof(tapIntStat));
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

	if (accX > 2047)
		accX = (accX - 4096);
	if (accY > 2047)
		accY = (accY - 4096);
	if (accZ > 2047)
		accZ = (accZ - 4096);
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
	setAccConfig0();
	nrf_delay_us(200);
	setAccConfig1Tap();
	// setAccConfig1();
	nrf_delay_us(200);
	setIntConfig0();
	nrf_delay_us(200);
	setIntConfig1();
	nrf_delay_us(200);
	setInt1Map();
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

	saveBMA400(accX, accY, accZ);
}

const nrf_drv_timer_t timerSelect3 = NRF_DRV_TIMER_INSTANCE(3);

static volatile uint8_t tap_count = 0;
void handlerTimer3(nrf_timer_event_t event_type, void *p_context)
{
		nrf_gpio_pin_toggle(18);
    if (tap_count == 1)
		{
			NRF_LOG_INFO("Single Tap Detected Single Tap Detected Single Tap Detected Single Tap Detected Single Tap Detected");
			NRF_LOG_FLUSH();
    }
		else if (tap_count == 2)
		{
			NRF_LOG_INFO("Double Tap Detected Double Tap Detected Double Tap Detected Double Tap Detected Double Tap Detected");
			NRF_LOG_FLUSH();
    }
		else if (tap_count >= 3)
		{
			NRF_LOG_INFO("Multi Tap Detected Multi Tap Detected Multi Tap Detected Multi Tap Detected Multi Tap Detected ");
			NRF_LOG_FLUSH();
    }
		
    tap_count = 0;
    nrf_drv_timer_disable(&timerSelect3);
}

void initBMA400TapTimer()
{
    uint32_t BMA400Time_ms = 2000; // Time Setting
    uint32_t timeTicks;
    uint32_t err_code = NRF_SUCCESS;

    nrf_drv_timer_config_t configTimer3 = NRF_DRV_TIMER_DEFAULT_CONFIG;
    err_code = nrf_drv_timer_init(&timerSelect3, &configTimer3, handlerTimer3); // Use Timer3
    APP_ERROR_CHECK(err_code);

    timeTicks = nrf_drv_timer_ms_to_ticks(&timerSelect3, BMA400Time_ms);

    nrf_drv_timer_extended_compare(
        &timerSelect3, NRF_TIMER_CC_CHANNEL3, timeTicks, NRF_TIMER_SHORT_COMPARE3_CLEAR_MASK, true);
}

void BMA400Tap()
{
		tap_count++;
		nrf_gpio_pin_toggle(17);

		if (!nrf_drv_timer_is_enabled(&timerSelect3))
			{
        nrf_drv_timer_enable(&timerSelect3); // Timer Start
    }
}
