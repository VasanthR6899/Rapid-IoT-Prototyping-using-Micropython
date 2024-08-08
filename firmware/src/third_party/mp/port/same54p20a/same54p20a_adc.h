
#ifndef _SAME54_PY_ADC_

#define _SAME54_PY_ADC_

#include "definitions.h"

typedef enum
{
    ADC0 = 0,
    ADC1 = 1
} ADC_NUM;

void adc_enable(int adc_instance);
uint16_t adc_read(ADC_NUM adc_num);
uint32_t adc_read_voltage(ADC_NUM adc_num);

#endif