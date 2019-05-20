/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"


#include "diag/Trace.h"

#include "wizchip_conf.h"
#include "socket.h"
#include "delay.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F1 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


void InitSpi1(void);
uint8_t SPI_RW(uint8_t data);
void SPI_Send(uint8_t data);
uint8_t SPI_Recv(void);
void ChipSelect(void);
void ChipDeselect(void);
void Sock(void);
void SetInterrupt(void);
void InterruptExec(void);

void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;

  RCC_DeInit();
  RCC_HSEConfig(RCC_HSE_ON);
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(SUCCESS == HSEStartUpStatus)
  {

    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    FLASH_SetLatency(FLASH_Latency_2);


    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLK2Config(RCC_HCLK_Div1);
    RCC_PCLK1Config(RCC_HCLK_Div2);

#ifndef STM32F10X_CL
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
#else

    RCC_PREDIV2Config(RCC_PREDIV2_Div5);
    RCC_PLL2Config(RCC_PLL2Mul_8);

    RCC_PLL2Cmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET);


    RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
#endif

    RCC_PLLCmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    while(RCC_GetSYSCLKSource() != 0x08);
    RCC_ClockSecuritySystemCmd(ENABLE);

 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
					| RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
					| RCC_APB2Periph_AFIO, ENABLE);
  }
}

uint8_t retVal, sockStatus;
	int16_t rcvLen;
	uint8_t rcvBuf[20], bufSize[] = {2, 2, 2, 2, 2, 2, 2, 2};

int
main(int argc, char* argv[])
{

	RCC_Configuration();
	SysTick_Init();

	trace_printf("Initializing SPI\n");
	InitSpi1();
	trace_printf("SPI Initialized\n");

	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	delay_ms(5);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	delay_ms(70);


	reg_wizchip_cs_cbfunc(ChipSelect, ChipDeselect);
	reg_wizchip_spi_cbfunc(SPI_Recv, SPI_Send);


	trace_printf("Functions registered, initializing W5500\n");




	wizchip_init(bufSize, bufSize);

	trace_printf("W5500 has been initialized.\n");

	wiz_NetInfo netInfo = { .mac 	= {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef},	// Mac address
				                          .ip 	= {192, 168, 0, 120},					// IP address
				                          .sn 	= {255, 255, 255, 0},					// Subnet mask
				                          .gw 	= {192, 168, 0, 250}};					// Gateway address


	wizchip_setnetinfo(&netInfo);
	trace_printf("Trying to obtain IP address...\n");
	wizchip_getnetinfo(&netInfo);
	trace_printf("Assigned IP Address: %d.%d.%d.%d\n", netInfo.ip[0], netInfo.ip[1], netInfo.ip[2], netInfo.ip[3]);
	delay_ms(1000);

	Sock();


  while (1)
    {
       // Add your code here.
    }
}


void Sock() {
	reconnect:
	  /* Open socket 0 as TCP_SOCKET with port 5000 */
	  if((retVal = socket(0, Sn_MR_TCP, 5000, 0)) == 0) {
		  /* Put socket in LISTEN mode. This means we are creating a TCP server */
		  if((retVal = listen(0)) == SOCK_OK) {
			  /* While socket is in LISTEN mode we wait for a remote connection */
			  while(sockStatus = getSn_SR(0) == SOCK_LISTEN)
				  delay_ms(100);
			  /* OK. Got a remote peer. Let's send a message to it */
			  while(1) {
				  /* If connection is ESTABLISHED with remote peer */
				  if(sockStatus = getSn_SR(0) == SOCK_ESTABLISHED) {
					  uint8_t remoteIP[4];
					  uint16_t remotePort;
					  /* Retrieving remote peer IP and port number */
					  getsockopt(0, SO_DESTIP, remoteIP);
					  getsockopt(0, SO_DESTPORT, (uint8_t*)&remotePort);
					  trace_printf("Client connected: %d.%d.%d.%d:%d\n", remoteIP[0], remoteIP[1], remoteIP[2], remoteIP[3], remotePort);

					  /* Let's send a welcome message and closing socket */
					  if(retVal = send(0, "W5500>Hi!", strlen("W5500>Hi!")) == (int16_t)strlen("W5500>Hi!")) {
						  trace_printf("Sent welcome message, now waiting for messages...\n");
					  	  int received = 0;
					  	  char buffer[250] = {0}, sendBuffer[250] = {0};
					  	  int cnt = 0;
					  	  while(0 != (received = recv(0, &buffer, 250))) {
					  		  if (cnt > 100)
					  			  cnt = 0;
					  		  if (received == 0 || received < 0)
									  break;
					  		  memcpy(buffer + received, '\0', 1);
					  		  trace_printf("Incoming>%s\n", buffer);
					  		  if (strstr(buffer, "req:") > 0)
					  			sprintf(sendBuffer, "W5500>%s", buffer);
					  		  else
					  			  sprintf(sendBuffer, "W5500>server_%d", cnt++);
					  		  send(0, sendBuffer, strlen(sendBuffer));
					  		  trace_printf("%s\n", sendBuffer);
					  		  memcpy(buffer, 0, 250);
					  		  memcpy(sendBuffer, 0, 250);

					  	  }
					  }
					  else { /* Ops: something went wrong during data transfer */
						  trace_printf("Data transfer error\n");
					  }
					  //break;
				  }
				  else { /* Something went wrong with remote peer, maybe the connection was closed unexpectedly */
					  trace_printf("Client connection error or client closed the connection\n");
					  break;
				  }
			  }

		  } else /* Ops: socket not in LISTEN mode. Something went wrong */
			  trace_printf("sock_listen error \n");
	  } else { /* Can't open the socket. This means something is wrong with W5100 configuration: maybe SPI issue? */
		  trace_printf("Cannot open socket\n");
	  }

	  /* We close the socket and start a connection again */
	  disconnect(0);
	  close_s(0);
	  goto reconnect;
}

void InitSpi1() {
	SPI_InitTypeDef SPI_Struct;
	GPIO_InitTypeDef GPIO_Struct;
	//RCC->APB2ENR |= ( 1<<12 ) | ( 1<<0 ) | ( 1<<2 ) | ( 1<<3 );
	SPI_Struct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_Struct.SPI_DataSize = SPI_DataSize_8b;
	SPI_Struct.SPI_CPOL = SPI_CPOL_Low;
	SPI_Struct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_Struct.SPI_NSS = SPI_NSS_Soft;

	SPI_Struct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Struct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_Struct.SPI_Mode = SPI_Mode_Master;
	SPI_Struct.SPI_CRCPolynomial = 7;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);


	SPI_Init(SPI1, &SPI_Struct);
	SPI_Cmd(SPI1, ENABLE);

	GPIO_Struct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_Struct);

	GPIO_Struct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_Struct);

	GPIO_Struct.GPIO_Pin = GPIO_Pin_4;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_Struct);

	GPIO_Struct.GPIO_Pin = GPIO_Pin_1;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_Struct);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);


}

void SetInterrupt() {
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	GPIO_InitTypeDef GPIO_Struct;

	GPIO_Struct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Struct.GPIO_Pin = GPIO_Pin_1;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_Struct);

	AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI1_PB;

	EXTI->IMR |= (1 << 1);
	EXTI->FTSR |= (1 << 1);

	NVIC_InitTypeDef NVIC_Struct;

	NVIC_Struct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_Struct.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Struct);

}

void EXTI1_IRQHandler() {
	if (EXTI_GetITStatus(EXTI_Line1) != RESET) {

		InterruptExec();

		EXTI_ClearITPendingBit(EXTI_Line1);
		wizchip_clrinterrupt(IK_SOCK_0);
	}
}

void InterruptExec() {
	trace_printf("here\n");
	//setSn_IR(0, 0x1f);

}


void SPI_Send(uint8_t data) {
	SPI_RW(data);
}

uint8_t SPI_Recv(void) {
	return SPI_RW(0xff);
}

uint8_t SPI_RW(uint8_t data) {
	/*!< Loop while DR register in not emplty */
	  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	  /*!< Send byte through the SPI2 peripheral */
	  SPI_I2S_SendData(SPI1, data);

	  /*!< Wait to receive a byte */
	  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	  /*!< Return the byte read from the SPI bus */
	  return SPI_I2S_ReceiveData(SPI1);
}

void ChipSelect() {
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}

void ChipDeselect() {
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
