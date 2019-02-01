#include "stm32f10x.h"
#include <math.h>

float T = 0.02; //ms 
float Th; // entre 1 et 2 ms 
float Umax = (2.0/20.0)*3.3; //tension max du servomoteur 
float Umin = (1.0/20.0)*3.3; //tension min du servomoteur
float Fmax= 5; //frequence max entrée
float k; //coeff de la loi entrée sortie; 
 
int Duty; //dutycycle 
float U; //tension appliquée au servo 
int Fproc = 108000000 ;

void Config_Port_Servo(void)
{
	GPIOA->CRL |= (0x1 << 4); 
	//Port PA1 sortie PWM timer 2 channel 2 en mode push pull output 
}

void Config_TimerPWM(void) //channel 2 timer 2
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //init du timer 2

	TIM2->CCMR2 |= 0x6; 
	//PWM mode 1 
	
	TIM2->PSC = 2039; // on reduit la freq

	
	TIM2->CR1 |= 1; 
	
}

void Config_PWM(float Freq) 
{
	k= (float)(Umax/Fmax);
	U= Umin + (k*Freq)/2.0; 
	Th= (float)(U*T)/3.3;
	
	TIM2->ARR=1000 ; //pour avoir T=20 ms
	Duty = floor((Th*Fproc)/2040.0); //pour avoir le bon rapport et donc la bonne tension de sortie 
	TIM2->CCR1 = Duty; 
	TIM2->ARR=0; 
	
}
