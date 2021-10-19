/**
  ******************************************************************************
  * @file    main.c
  * @author  Omar MEGLOUD
  * @version V1.0
  * @date    01-December-2019
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lights_status.h"

unsigned char cnt = 99;
unsigned char redDuration = 20;
unsigned char greenDuration = 15;
unsigned char redStart = 0;
unsigned char greenStart = 0;
unsigned char pedMode = 0;

void Task1(void *pvParameters)
{
	unsigned char Flag = 0;
	for(;;)
	{
		if(redStart == 1)
		{
			redStart = 0;
			cnt = redDuration;
			Flag = 1;
		}
		else if(greenStart == 1)
		{
			greenStart = 0;
			cnt = greenDuration;
			Flag = 1;
		}

		if(Flag == 1)
		{
			vTaskDelay(pdMS_TO_TICKS(1000));
			cnt--;
			if(cnt == 0)
			{
				cnt = 99;
				Flag = 0;
			}
		}
	}
}

void Task2(void *pvParameters)
{
	unsigned char Pattern;
	unsigned char MSD, LSD;
	unsigned char SEGMENT[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
	// 7 segment display configuration
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitTypeDef seg7_init;
	memset(&seg7_init, 0, sizeof(seg7_init));

	seg7_init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	seg7_init.GPIO_Mode = GPIO_Mode_OUT;
	seg7_init.GPIO_OType = GPIO_OType_PP;
	seg7_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	seg7_init.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOE, &seg7_init);

	// Digit configuration
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef digit_init;
	memset(&digit_init, 0, sizeof(digit_init));

	digit_init.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	digit_init.GPIO_Mode = GPIO_Mode_OUT;
	digit_init.GPIO_OType = GPIO_OType_PP;
	digit_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	digit_init.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD, &digit_init);

	GPIO_WriteBit(GPIOD, GPIO_Pin_9, Bit_RESET);	// Disable MS DIGIT
	GPIO_WriteBit(GPIOD, GPIO_Pin_10, Bit_RESET);	// Disable LS DIGIT

	for(;;)
	{
		if(cnt != 99)
		{
			// MSD Display
			MSD = cnt / 10;
			GPIOE->ODR = ~(SEGMENT[MSD]);
			GPIO_WriteBit(GPIOD, GPIO_Pin_9, Bit_SET);
			vTaskDelay(pdMS_TO_TICKS(10));
			GPIO_WriteBit(GPIOD, GPIO_Pin_9, Bit_RESET);
			// LSD Display
			LSD = cnt % 10;
			GPIOE->ODR = ~(SEGMENT[LSD]);
			GPIO_WriteBit(GPIOD, GPIO_Pin_10, Bit_SET);
			vTaskDelay(pdMS_TO_TICKS(10));
			GPIO_WriteBit(GPIOD, GPIO_Pin_10, Bit_RESET);
		}
	}
}

void Task3(void *pvParameters)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef led_init;
	memset(&led_init, 0, sizeof(led_init));

	led_init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	led_init.GPIO_Mode = GPIO_Mode_OUT;
	led_init.GPIO_OType = GPIO_OType_PP;
	led_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	led_init.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &led_init);

	for(;;)
	{
		redStart = 0; greenStart = 0;
		lights_status_3();
		greenStart = 1;
		vTaskDelay(pdMS_TO_TICKS(15000));
		lights_status_4();
		vTaskDelay(pdMS_TO_TICKS(2000));
		lights_status_2();
		redStart = 1;
		vTaskDelay(pdMS_TO_TICKS(2000));
		lights_status_1();
		vTaskDelay(pdMS_TO_TICKS(15000));
		lights_status_2();
		vTaskDelay(pdMS_TO_TICKS(2000));
		lights_status_4();
		vTaskDelay(pdMS_TO_TICKS(2000));

		if(pedMode == 1)
		{
			lights_status_5();
			pedMode = 0;
			vTaskDelay(pdMS_TO_TICKS(10000));
		}
	}
}

void Task4(void *pvParameters)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef button_init;
	memset(&button_init, 0, sizeof(button_init));

	button_init.GPIO_Pin = GPIO_Pin_5;
	button_init.GPIO_Mode = GPIO_Mode_IN;
	button_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	button_init.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &button_init);

	for(;;)
	{
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5))
		{
			pedMode = 1;
		}
	}

}

int main(void)
{
	RCC_DeInit();
	SystemCoreClockUpdate();

	xTaskCreate(
			(TaskFunction_t) Task1,
			"LED Controller",
			configMINIMAL_STACK_SIZE,
			NULL,
			10,
			NULL
	);

	xTaskCreate(
			(TaskFunction_t) Task2,
			"7-seg Controller",
			configMINIMAL_STACK_SIZE,
			NULL,
			10,
			NULL
	);

	xTaskCreate(
			(TaskFunction_t) Task3,
			"Main programmer",
			configMINIMAL_STACK_SIZE,
			NULL,
			10,
			NULL
	);

	xTaskCreate(
			(TaskFunction_t) Task4,
			"Pedestrian Controller",
			configMINIMAL_STACK_SIZE,
			NULL,
			10,
			NULL
	);

	vTaskStartScheduler();

	for(;;);
}
