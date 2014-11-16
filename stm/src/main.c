#include "stm32f4xx_conf.h"
#include <math.h>

volatile short MyDelay;

/* For 50 Hz PWM signal Prescaler and Period Values */
uint16_t Prescaler = 84;
uint32_t Period = 20000;



void Pin_Config(void);
void Timer3_PWM_Output_Config(uint32_t PeriodValue, uint16_t PrescalerValue);


void DelayMs(unsigned int ms);

int main(void)
{
    SystemInit();
    SysTick_Config(SystemCoreClock / 1000); //1milisaniyelik system clock elde edelim

    Pin_Config();
    Timer3_PWM_Output_Config(Period, Prescaler);


    unsigned int sayac1=0,sayac2=0,sayac3=0,sayac4=0;
    char durum1=1,durum2=1,durum3=1,durum4=1; //1=artma, 0=eksilme

    unsigned int aci=0;
    unsigned int sifirKonumu=500;
    unsigned int pwm=0;

    aci=0;
    pwm=(aci*10)+sifirKonumu;

    TIM_SetCompare1(TIM3,pwm);

    DelayMs(2000); //0 derecede 2 saniye bekle

    while (1)
    {

        if(durum1) //artirma modu
        {
            aci++; //aci 1'er 1'er artar
            if(aci>=180)durum1=0; //180 dereceye ulastiysa eksiltme moduna gec
        }
        else // eksiltme modu
        {
            aci--; //aci 1'er 1'er eksilir
            if(aci==0)durum1=1; //0 dereceye kadar dustuyse artirma moduna gec
        }

        pwm=(aci*10)+sifirKonumu; //pwm 500-550 iken 0 derece konumunda bekler

        TIM_SetCompare1(TIM3,pwm); //pwm'i guncelle

        DelayMs(10); //acinin her degismesinde 10ms bekler
    }
}

void Pin_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;


    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* GPIOC clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Connect TIM3 pins to AF2 */
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);

    /******************************************************************/
}

void Timer3_PWM_Output_Config(uint32_t PeriodValue, uint16_t PrescalerValue)
{

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = PeriodValue - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* PWM1 Mode configuration */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;

    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);


    TIM_ARRPreloadConfig(TIM3, ENABLE);

    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE);

}




void SysTick_Handler(void)
{
    if (MyDelay)
    {
        MyDelay--;
    }
}

void DelayMs(unsigned int ms)
{
    MyDelay=ms;
    while(MyDelay);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif
