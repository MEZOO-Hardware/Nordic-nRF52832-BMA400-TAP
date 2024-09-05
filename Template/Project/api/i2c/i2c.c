//
// Created by joey on 2024-06-18.
//

#include "i2c.h"

/* I2C instance. */
static nrf_drv_twi_t I2C0 = NRF_DRV_TWI_INSTANCE(0);

static volatile bool m_xfer_done = false;

void HandlerI2C(nrf_drv_twi_evt_t const *p_event, void *p_context)
{
	switch (p_event->type)
	{
	case NRF_DRV_TWI_EVT_DONE:
		m_xfer_done = true;
		if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX)
		{
		}
		break;

	default:
		break;
	}
}

void initI2C()
{
	ret_code_t err_code;

	const nrf_drv_twi_config_t configI2C =
		{
			.scl = I2C_SCL_PIN,
			.sda = I2C_SDA_PIN,
			.frequency = NRF_DRV_TWI_FREQ_400K,
			.interrupt_priority = APP_IRQ_PRIORITY_HIGH,
			.clear_bus_init = false};

	err_code = nrf_drv_twi_init(&I2C0, &configI2C, HandlerI2C, NULL);
	APP_ERROR_CHECK(err_code);

	nrf_drv_twi_enable(&I2C0);
}

ret_code_t writeI2C(uint8_t address, uint8_t *w_data, uint8_t w_length)
{
	ret_code_t err_code;

	m_xfer_done = false;
	err_code = nrf_drv_twi_tx(&I2C0, address, w_data, w_length, true);
	APP_ERROR_CHECK(err_code);
	while (m_xfer_done == false)
		;

	return err_code;
}

ret_code_t writeReadI2C(uint8_t address, uint8_t *w_data, uint8_t w_length, uint8_t *r_data, uint8_t r_length)
{
	ret_code_t err_code;

	m_xfer_done = false;
	err_code = nrf_drv_twi_tx(&I2C0, address, w_data, w_length, true);
	APP_ERROR_CHECK(err_code);
	while (m_xfer_done == false)
		;

	if (err_code == NRF_SUCCESS)
	{
		m_xfer_done = false;
		err_code = nrf_drv_twi_rx(&I2C0, address, r_data, r_length);
		APP_ERROR_CHECK(err_code);
		while (m_xfer_done == false)
			;
	}

	return err_code;
}
