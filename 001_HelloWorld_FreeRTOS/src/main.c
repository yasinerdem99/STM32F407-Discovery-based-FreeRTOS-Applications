#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "stdio.h"
#include <string.h>
#include <stdbool.h>


#include "FreeRTOS.h"
#include "task.h"

bool UART_ACCESS_KEY = 1;

char myMessage[200];

TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;

void Task1_Handler(void *params);
void Task2_Handler(void *params);

void Setup(void);
void GPIO_Config(void);
void USART_Config(void);
void printMessage(char *message);

int main(void)
{
	//1.adým sistem saati dahili saat olarak ayarlandý
	RCC_DeInit(); // HSI ON | HSE OFF PLL OFF | SystemClock 16MHz

	//2.adým sistem saati 16 Mhz olarak ayarlandý
	SystemCoreClockUpdate(); // SystemCoreClock = 16000000

	Setup();

	sprintf(myMessage,"merhaba yasin\r\n");
	printMessage(myMessage);

	//3.adým görev oluþturma
	xTaskCreate(Task1_Handler, "Görev 1", configMINIMAL_STACK_SIZE,NULL,4,&myTask1Handle); // 130 *4 = 520 byte
	xTaskCreate(Task2_Handler, "Görev 2", configMINIMAL_STACK_SIZE,NULL,4,&myTask2Handle); // 130 *4 = 520 byte

	vTaskStartScheduler();

  while (1)
  {

  }
}

void Task1_Handler(void *params)
{
	 while (1)
	  {
		 	if(UART_ACCESS_KEY == 1)
		 	{
		 		UART_ACCESS_KEY=0;
		 		printMessage("1.gorev\r\n");
		 		UART_ACCESS_KEY=1;
		 		taskYIELD();

		 	}
	  }
}

void Task2_Handler(void *params)
{

	while (1)
		  {
			 	if(UART_ACCESS_KEY == 1)
			 	{
			 		UART_ACCESS_KEY=0;
			 		printMessage("2.gorev\r\n");
			 		UART_ACCESS_KEY=1;
			 		taskYIELD();

			 	}
		  }
}
void Setup(void)
{
	GPIO_Config();
	USART_Config();
}
void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

	GPIO_Init(GPIOA,&GPIO_InitStruct);


}
void USART_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	USART_InitTypeDef USART_InitStruct;

	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No ;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength =USART_WordLength_8b;

	USART_Init(USART2,&USART_InitStruct);

	USART_Cmd(USART2,ENABLE);
}

void printMessage(char *message)
{
	for(int i = 0; i<strlen(message) ; i++)
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)!=SET);
		USART_SendData(USART2,message[i]);

	}
}



/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
