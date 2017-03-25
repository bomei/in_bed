#include "bolib.h"
#include "stm32f10x_usart.h"
#include "string.h"

//-----------------------------here below is uart part--------------------------------------
#if EN_USART1_RX
	u8 USART1_RX_BUF[USART_REC_LEN];
	u16 USART1_RX_STA=0;

	void uart1_init(u32 bound){
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);

		//USART1_TX......GPIOA.9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;						//PA.9
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;					//mux push-pull output
		GPIO_Init(GPIOA, &GPIO_InitStructure);							//init PA.9

		//USART1_RX......GPIOA.10
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;						//PA.10
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//floating input
		GPIO_Init(GPIOA, &GPIO_InitStructure);							//init PA.10

		//USART1 NVIC config
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;			//priority
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
		NVIC_Init(&NVIC_InitStructure);

		//USART init

		USART_InitStructure.USART_BaudRate = bound;						//baudrate
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//8-b-data
		USART_InitStructure.USART_StopBits = USART_StopBits_1;			//1-b-stop
		USART_InitStructure.USART_Parity = USART_Parity_No;				//no parity
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//rx and tx mode

		USART_Init(USART1, &USART_InitStructure); 						//init uart1
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);					//enabel interrupt
		USART_Cmd(USART1,ENABLE);                    					//enable uart1
	}

	void USART1_send_buf(char *buf){
		int len=strlen(buf);
		int i=0;
		char ch;
		for(i=0;i<len;++i){
			ch=buf[i];
			while((USART1->SR&0X40)==0);
			USART_SendData(USART1, ch);
		}

	}

	void USART1_IRQHandler(void){
		u8 Res;
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
			Res =USART_ReceiveData(USART1);									//read rx data
			
			if((USART1_RX_STA&0x8000)==0){									//rx unfinish
				if(USART1_RX_STA&0x4000){									//rx 0x0d
					if(Res!=0x0a)
						USART1_RX_STA=0;										//rx error, restart
					else 
						USART1_RX_STA|=0x8000;								//rx done
				}
				else{
					if(Res==0x0d)
						USART1_RX_STA|=0x4000;
					else{
						USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res;
						USART1_RX_STA++;
						if(USART1_RX_STA>(USART_REC_LEN-1))
							USART1_RX_STA=0;									//rx error, restart
					}
				}
			}
		}
	}
#endif
