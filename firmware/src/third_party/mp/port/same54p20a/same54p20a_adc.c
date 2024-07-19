

#include "same54p20a_adc.h"
#include "definitions.h"


#define ADC_VREF                (3.3)
#define ADC_MAX_COUNT           (4095U)



void adc_enable(int adc_instance)
{

	switch (adc_instance)
    {
        case 0:    /*AIN12*/
            
            ADC0_Enable();
            break;
            
        case 1:    /*AIN6*/
            
            ADC1_Enable();
            break;
          
        default:
            break;
    }  
    

}

uint16_t adc_read(ADC_NUM adc_num)
{

    uint16_t adc_count;
    if(adc_num == 1)
    {
        ADC1_ConversionStart();
        
        while(!ADC1_ConversionStatusGet());
            
        adc_count = ADC1_ConversionResultGet();


        return adc_count;
        
    }
    else if(adc_num == 0)
    {
        ADC0_ConversionStart();
                
        while(!ADC0_ConversionStatusGet());
            
        adc_count = ADC0_ConversionResultGet();

        return adc_count;
        
    }
}

uint32_t adc_read_voltage(ADC_NUM adc_num)
{
    uint16_t adc_count;
    float input_voltage;
    
    if(adc_num == 1 )
    {
        ADC1_ConversionStart();
        
        while(!ADC1_ConversionStatusGet());
    
        adc_count = ADC1_ConversionResultGet();

        input_voltage = (float)adc_count * ADC_VREF / ADC_MAX_COUNT * 1000000 ;
                
        return input_voltage;
    
    }
    
    else if(adc_num == 0)
    {
        ADC0_ConversionStart();
        
        while(!ADC0_ConversionStatusGet());
    
        adc_count = ADC0_ConversionResultGet();

        input_voltage = adc_count * ADC_VREF / ADC_MAX_COUNT * 1000000;
    
        return input_voltage;
    
    }
    
    
}
 