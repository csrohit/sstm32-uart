/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Rohit Nimkar <nehalnimkar@gmail.com> <https://csrohit.github.io>
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * This software component is licensed by Rohit Nimkar under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at: opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
#include <stdint.h>
#include <stdio.h>
#include <stm32f1xx.h>

#include <time.h>
#include <main.h>

#define RX_BUFFER_LENGTH 20U
#define TX_BUFFER_LENGTH 13U

const char* msg = "Hello world\r\n\0";

char buff[RX_BUFFER_LENGTH];


/**
 * @brief Configure DMA1 channel 4 to work with USART1 transmitter
 * It reads from memory and writes to USART data register
 */
void dma_usart_tx_init(void){
	// enable clock for dma peripheral registers
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;

	// USART TX is mapped to DMA1 channel 4
	// set peripheral address to USART data register
	DMA1_Channel4->CPAR = (uint32_t) &USART1->DR;

	// set memory address to address of string
	DMA1_Channel4->CMAR = (uint32_t) msg;

	// set number od dma transactions
	DMA1_Channel4->CNDTR = RX_BUFFER_LENGTH;

	// set memory address incement by 1byte
	DMA1_Channel4->CCR |= DMA_CCR_MINC;

	// enable circular mode
	DMA1_Channel4->CCR |= DMA_CCR_CIRC;

	// set data transfer direction - memory -> peripheral
	DMA1_Channel4->CCR |= DMA_CCR_DIR;
}

/**
 * @brief Enable DMA to accept request for channel 4
 */
void dma_usart_tx_enable(void){
	DMA1_Channel4->CCR |= DMA_CCR_EN;
}

void usart1_init(void){
	// enable clock for GPIOA and USART1
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN;

    // reset pin configurations for PA9 and PA10
    GPIOA->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_MODE9 | GPIO_CRH_CNF10 | GPIO_CRH_CNF9);

    // PA9 as Output@50Hz Alternate function
    GPIOA->CRH |= GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1 | GPIO_CRH_CNF9_1;

    // PA10 as floating input
    GPIOA->CRH |= GPIO_CRH_CNF10_0;

	// set baud rate as 9600
    uint32_t baud = (uint32_t)(SystemCoreClock / 9600);
    USART1->BRR = baud;

	// Enable transmitter
	USART1->CR1 |= USART_CR1_TE;

	// Enable DMA mode for transmitter
    USART1->CR3 |= USART_CR3_DMAT;
}

/**
 * @brief Enable USART1 prescalers and output
 */
void usart1_enable(void){
	USART1->CR1 |= USART_CR1_UE;
}

int main(void)
{
	SysTick_Config(SystemCoreClock/1000);
	usart1_init();
	dma_usart_tx_init();
	dma_usart_tx_enable();
	usart1_enable();

	while (1)
	{	
		
	}
	
	
}
