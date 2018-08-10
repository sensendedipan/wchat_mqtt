
#include "adc_driver.h"


/**
  * @brief  adc init
  * @retval None
  */
void adcInit(void) 
{
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
   
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);  
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);  
    ADC_TempSensorVrefintCmd(ENABLE);
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    ADC_CommonInit(&ADC_CommonInitStructure);
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_Cmd(ADC1, ENABLE);
	debug("adc initialization OK ! \n");
}



/**
  * @brief  get NTC temperature
  * @note	value:
  * @retval temperature
  */
int8_t getTemperature(void)
{   
    int16_t  temp;
    int32_t  adcNow;
    static int16_t tempLast = 20;
    
    ADC_RegularChannelConfig(ADC1, 16, 1, ADC_SampleTime_480Cycles);			      
	ADC_SoftwareStartConv(ADC1);			 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

	adcNow = ADC_GetConversionValue(ADC1);
    temp = (adcNow*3300/4096-760)/2.5 + 25 - 15;

    tempLast = tempLast*0.75f + temp*0.25f;
    
    if (tempLast > 100) {
        tempLast = 100;
        
    } else if (tempLast < -45) {
        tempLast = -45;
    }
    
    return (int8_t)tempLast;

}




