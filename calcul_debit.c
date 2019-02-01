#include "stm32f10x.h"

void Config_Timer_Input(void) 
{
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; //init du timer 1
	TIM1->CR1 &= ~(0x1);
	TIM1->PSC = 26999;  //Fprocesseur =108MHz
	TIM1->ARR = 19999; //configuration pour avoir un timer de 5s
}

void Attente_Compteur_Debit(void)
{
	while(! (GPIOA->IDR & (1 << 0))) //tant que le bouton WKUP n'est pas appuyé
	{
		//boucle d'attente
	}
	TIM1->CR1 = TIM1->CR1 | ( 1 << 0); //demarrage du compteur
}

void Config_Interruption(void) 
{
	TIM1->DIER |= (1<<0); //autoristation de l'interruption localement
	NVIC->ISER[0] |= (1<<27); //autorisation de transmission de l'iT 
}

void Calcul_Frequence(float *Ptr_Debit) 
{
	/*RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //init du timer 2
	TIM2->CR2 |= (1<<0); 
	TIM2->CCMR2 &= ~(1<<8); //config en input PWM 
	TIM2->CCMR2 |= (1<<9); 
	Debit = TIM2->CCR2; */
	
	int valeur_precedente = 0; 
	int valeur_actuelle; 
	int compteur=0;
	
	while(! TIM1->SR & TIM_SR_UIF) //tant qu'il n y a pas interruption
	{
		valeur_actuelle = ((GPIOA->IDR & (1 << 8)) >> 8);
		
		if (valeur_actuelle != valeur_precedente) 
		{
			compteur++; 												//on compte les changements d'etat de l'entrée
			valeur_precedente=valeur_actuelle; 
		}
		
	}
	if (compteur>175) 
	{
		compteur=175; 
	}
	*Ptr_Debit= (float)(compteur/35.0);
	
}


