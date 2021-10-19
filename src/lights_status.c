#include "lights_status.h"
#include "stm32f4xx.h"

void lights_status_1(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5, Bit_RESET);  	// GREEN1 YELLOW1 YELLOW2 RED2 OFF
	GPIO_WriteBit(GPIOA, GPIO_Pin_2 | GPIO_Pin_3, Bit_SET);  	// RED1 GREEN2 ON
}

void lights_status_2(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5, Bit_RESET);  	// GREEN1 YELLOW1 GREEN2 RED2 OFF
	GPIO_WriteBit(GPIOA, GPIO_Pin_2 | GPIO_Pin_4, Bit_SET);  	// RED1 YELLOW2 ON
}

void lights_status_3(void)
{
	// Direction 1 lights
	GPIO_WriteBit(GPIOA, GPIO_Pin_0 | GPIO_Pin_5, Bit_SET);  	// GREEN1 RED2 ON
	GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_RESET);	// YELLOW1 RED1 GREEN2 YELLOW2
}

void lights_status_4(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4, Bit_RESET);	// GREEN1 RED1 GREEN2 YELLOW2
	GPIO_WriteBit(GPIOA, GPIO_Pin_1 | GPIO_Pin_5 , Bit_SET);		// YELLOW1 RED2 ON
}

void lights_status_5(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4, Bit_RESET);	// GREEN1 YELLOW1 GREEN2 YELLOW2
	GPIO_WriteBit(GPIOA, GPIO_Pin_2 | GPIO_Pin_5 , Bit_SET);		// RED1 RED2 ON
}
