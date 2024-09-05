//
// Created by joey on 2024-06-18.
//

#include "adc.h"

/* Timer instance. */
const nrf_drv_timer_t timerSelect2 = NRF_DRV_TIMER_INSTANCE(2);

void handlerTimer2(nrf_timer_event_t event_type, void *p_context)
{
}

void saadc_sampling_event_init()
{
    ret_code_t err_code;

    err_code = nrf_drv_ppi_init();
    APP_ERROR_CHECK(err_code);

    uint32_t time_ms = 4; // Timer Setting
    uint32_t time_ticks;

    nrf_drv_timer_config_t configTimer2 = NRF_DRV_TIMER_DEFAULT_CONFIG;
    configTimer2.bit_width = NRF_TIMER_BIT_WIDTH_32;
    err_code = nrf_drv_timer_init(&timerSelect2, &configTimer2, handlerTimer2); // Use Timer2
    APP_ERROR_CHECK(err_code);

    time_ticks = nrf_drv_timer_ms_to_ticks(&timerSelect2, time_ms);
    nrf_drv_timer_extended_compare(&timerSelect2,
                                   NRF_TIMER_CC_CHANNEL2,
                                   time_ticks,
                                   NRF_TIMER_SHORT_COMPARE2_CLEAR_MASK,
                                   false);
    nrf_drv_timer_enable(&timerSelect2);

    uint32_t timer_compare_event_addr = nrf_drv_timer_compare_event_address_get(&timerSelect2,
                                                                                NRF_TIMER_CC_CHANNEL2);

    uint32_t saadc_sample_task_addr = nrf_drv_saadc_sample_task_get();

    /* setup ppi channel so that timer compare event is triggering sample task in SAADC */
    err_code = nrf_drv_ppi_channel_alloc(&m_ppi_channel);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_ppi_channel_assign(m_ppi_channel, timer_compare_event_addr, saadc_sample_task_addr);
    APP_ERROR_CHECK(err_code);
}

void saadc_sampling_event_enable()
{
    ret_code_t err_code = nrf_drv_ppi_channel_enable(m_ppi_channel);

    APP_ERROR_CHECK(err_code);
}

void readADC(nrf_drv_saadc_evt_t const *p_event)
{
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
        ret_code_t err_code;

        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAMPLES_IN_BUFFER);
        APP_ERROR_CHECK(err_code);

        //        int i;
        //        NRF_LOG_INFO("ADC event number: %d", (int)m_adc_evt_counter);

        //        for (i = 0; i < SAMPLES_IN_BUFFER; i++)
        //        {
        //            NRF_LOG_INFO("%d", p_event->data.done.p_buffer[i]);
        //        }
        //        m_adc_evt_counter++;

        //				rawDataADC = p_event->data.done.p_buffer[0];
        //        resultValueADC = ADC_RESULT_IN_VOLTS(rawDataADC);
    }
}

void initADC()
{
    ret_code_t err_code;
    nrf_saadc_channel_config_t channel_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN2);

    err_code = nrf_drv_saadc_init(NULL, readADC);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_channel_init(0, &channel_config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[0], SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);

    saadc_sampling_event_init();
    saadc_sampling_event_enable();
}
