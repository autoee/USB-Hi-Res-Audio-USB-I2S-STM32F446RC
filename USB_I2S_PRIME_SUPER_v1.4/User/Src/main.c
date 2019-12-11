/*
********************************************************************************
* COPYRIGHT(c) ЗАО «ЧИП и ДИП», 2019
* 
* Программное обеспечение предоставляется на условиях «как есть» (as is).
* При распространении указание автора обязательно.
********************************************************************************
*/


#include "usb_audio.h"
#include "audio_configuration.h"
#include "usbd_audio_if.h"
#include "usbd_desc.h"


USBD_HandleTypeDef USBD_Device;
extern USBD_AUDIO_InterfaceCallbacksfTypeDef audio_class_interface;


int main(void)
{
  USB_I2S_Init();
  
  while (1)
  {
    __WFI();
  }
}
//------------------------------------------------------------------------------
void USB_I2S_Init(void)
{
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  HAL_InitTick(TICK_INT_PRIORITY);
  
  //HSE, PLL 168 MHz
  FLASH->ACR = FLASH_ACR_LATENCY_5WS | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;
  
  RCC->CR |= RCC_CR_HSEON;
  while((RCC->CR & RCC_CR_HSERDY) == 0);
    
  RCC->PLLCFGR = RCC_PLLCFGR_PLLR_1 | RCC_PLLCFGR_PLLQ_0 | RCC_PLLCFGR_PLLQ_1 | RCC_PLLCFGR_PLLQ_2 | \
                 RCC_PLLCFGR_PLLSRC | RCC_PLLCFGR_PLLN_4 | RCC_PLLCFGR_PLLN_6 | RCC_PLLCFGR_PLLN_8 | RCC_PLLCFGR_PLLM_3;
    
  RCC->CFGR = RCC_CFGR_PPRE2_DIV2 | RCC_CFGR_PPRE1_DIV4;
    
  RCC->CR |= RCC_CR_PLLON;
  while((RCC->CR & RCC_CR_PLLRDY) == 0);
  
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL);
  
  //добавить для всех используемых выводов
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;
  while(((RCC->AHB1ENR & RCC_AHB1ENR_GPIOAEN) != RCC_AHB1ENR_GPIOAEN)
     || ((RCC->AHB1ENR & RCC_AHB1ENR_GPIOBEN) != RCC_AHB1ENR_GPIOBEN)
     || ((RCC->AHB1ENR & RCC_AHB1ENR_GPIOCEN) != RCC_AHB1ENR_GPIOCEN));
    
  //выводы конфигурации
  ConfigGPIOs_Init();
  //задержка, если нужна
  InitDelay();
  //USB
  USBD_Init(&USBD_Device, &AUDIO_Desc, 0);
  //аудио конфигурация
  AudioConfig_Init();
  OUTClk_Init();
  //USB
  USBD_RegisterClass(&USBD_Device, USBD_AUDIO_CLASS);
  USBD_AUDIO_RegisterInterface(&USBD_Device, &audio_class_interface);
  USBD_Start(&USBD_Device);
}
//------------------------------------------------------------------------------
void Error_Handler(void)
{
   
}
//------------------------------------------------------------------------------
void USBD_error_handler(void)
{
  Error_Handler();
}

