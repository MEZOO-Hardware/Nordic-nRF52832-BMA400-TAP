//
// Created by joey on 2024-06-18.
//

#include "pinChangeInterrupt.h"

volatile bool flag50HzBMA400 = false;

void enableBMA400ReadyPin50Hz()
{
		flag50HzBMA400 = true;
}

void disableBMA400ReadyPin50Hz()
{
		flag50HzBMA400 = false;
}

bool isBMA400ReadyPin50Hz()
{
		return flag50HzBMA400;
}
void handlerMAX30205(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
}

void handlerBMA400(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
		enableBMA400ReadyPin50Hz();
}

void handlerAD7171(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
}

void handlerBMA400Tap(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
		NRF_LOG_INFO("Single tap detected");
		NRF_LOG_FLUSH();
}

void initRDYPinMAX30205()
{
		ret_code_t err_code;
		
		nrf_drv_gpiote_in_config_t configMAX30205 = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    configMAX30205.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(P12, &configMAX30205, handlerMAX30205);
    APP_ERROR_CHECK(err_code);
		
    nrf_drv_gpiote_in_event_enable(P12, true);
}

void initRDYPinBMA400()
{
		ret_code_t err_code;
	
		nrf_drv_gpiote_in_config_t configBMA400 = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    configBMA400.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(12, &configBMA400, handlerBMA400);
    APP_ERROR_CHECK(err_code);
		
    nrf_drv_gpiote_in_event_enable(12, true);
}

void initRDYPinAD7171()
{
		ret_code_t err_code;
		
		nrf_drv_gpiote_in_config_t configAD7171 = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    configAD7171.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(P11, &configAD7171, handlerAD7171);
    APP_ERROR_CHECK(err_code);
		
    nrf_drv_gpiote_in_event_enable(P11, true);
}

void initRDYPinBMA400Tap()
{
		ret_code_t err_code;
		
		nrf_drv_gpiote_in_config_t configBMA400Tap = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    configBMA400Tap.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(P13, &configBMA400Tap, handlerBMA400Tap);
    APP_ERROR_CHECK(err_code);
		
    nrf_drv_gpiote_in_event_enable(P13, true);
}

void initPinChangeInterrupt()
{
    ret_code_t err_code;
	
    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);
		
//		initRDYPinMAX30205();
		initRDYPinBMA400();
//		initRDYPinBMA400Tap();
//		initRDYPinAD7171();
}