#define RCGCGPIO (*((volatile unsigned long *) 0x400FE608))

#define RCGCADC (*((volatile unsigned long *) 0x400FE638))
#define GPIOAFSEL_PORTE (*((volatile unsigned long *) 0x40024420))
#define GPIODEN_PORTE (*((volatile unsigned long *) 0x4002451C))
#define GPIOAMSEL_PORTE (*((volatile unsigned long *) 0x40024528))
#define ADCACTSS_ADC0 (*((volatile unsigned long *) 0x40038000))
#define ADCEMUX_ADC0 (*((volatile unsigned long *) 0x40038014))
#define ADCSSMUX0_ADC0 (*((volatile unsigned long *) 0x40038040))
#define ADCPSSI_ADC0 (*((volatile unsigned long *) 0x40038028))
#define ADCRIS_ADC0 (*((volatile unsigned long *) 0x40038004))
#define ADCSSFIFO3_ADC0 (*((volatile unsigned long *) 0x400380A8))
#define ADCISC_ADC0 (*((volatile unsigned long *) 0x4003800C))

#define RCGCPWM (*((volatile unsigned long *) 0x400FE640))
#define RCC (*((volatile unsigned long *) 0x400FE060))
#define GPIOAFSEL_PORTF (*((volatile unsigned long *) 0x40025420))
#define GPIOPCTL_PORTF (*((volatile unsigned long *) 0x4002552C))
#define GPIODEN_PORTF (*((volatile unsigned long *) 0x4002551C))
#define PWM3CTL_M1 (*((volatile unsigned long *) 0x40029100))
#define PWM3GENA_M1 (*((volatile unsigned long *) 0x40029120))
#define PWM3LOAD_M1 (*((volatile unsigned long *) 0x40029110))
#define PWM3COMA_M1 (*((volatile unsigned long *) 0x40029118))
#define PWMENABLE_M1 (*((volatile unsigned long *) 0x40029008))

void ADC_setup();
void PWM_setup();
int main(void)
{
    PWM_setup();
    ADC_setup();
    while(1)
    {

        ADCPSSI_ADC0 |= 0x8;//start sampling SS3 b'1000
        //while((ADCRIS_ADC0 & 8) == 0);//wait for SS3 complete sampling
        PWM3COMA_M1 = ADCSSFIFO3_ADC0;//led PWM change
        ADCISC_ADC0 = 0x8;//clear SS3
    }
}


void PWM_setup(){
    RCGCGPIO |= 0x20;//port F2 clock enable 100000
    RCGCPWM |= 0x2;//PWM module 1 clock enable
    RCC |= 0x00000000;//no clock divisor
    GPIOAFSEL_PORTF |= 0x4;//Alternative function for port F2
    GPIOPCTL_PORTF &= ~0x00000F00;//
    GPIOPCTL_PORTF |= 0x00000500;//set port F2 as PWM output
    GPIODEN_PORTF |= 0x4;//enable port F2 digital output
    PWM3CTL_M1 &= ~0x1;//disable before configuration
    PWM3GENA_M1 = 0x0000008C;//high when count is load, low when count is COMPA
    PWM3LOAD_M1 = 4096;
    PWM3CTL_M1 |= 0x1;//enable PWM generation, up-down mod
    PWMENABLE_M1 = 0x40;//channel 6 enable
}

void ADC_setup(){
    RCGCGPIO |= 0x10;//port E3 clock enable 10000
    RCGCADC |= 0x1;//ADC module0 clock enable
    GPIOAFSEL_PORTE |= 0x8;//port E3 alternative function
    GPIODEN_PORTE &= ~(0x8);//Port E3 disable digital
    GPIOAMSEL_PORTE |= 0x8;//port E3 available analog
    ADCACTSS_ADC0 &= ~(0x8);//disable sampling SS3
    ADCEMUX_ADC0 = 0xF000;//trigger SS3 with always sampling
    ADCSSMUX0_ADC0 = 0;//analog input is channel 0 = PORT E3
    ADCACTSS_ADC0 |= 0x8;//active sampling SS3
}
