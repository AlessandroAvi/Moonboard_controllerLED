#include "main.h"
#include "usart.h"
#include "debug.h"
#include <string.h>
#include <stdio.h>



void PRINTF(char str[50]){
  char msg[50];
  int msg_len;
  msg_len = sprintf(msg, str);
  HAL_UART_Transmit(&huart2, (uint8_t *)msg, msg_len, 5000);
}


void PRINTF8(char str[50], uint8_t val){
  char msg[50];
  int msg_len;
  msg_len = sprintf(msg, str,val);
  HAL_UART_Transmit(&huart2, (uint8_t *)msg, msg_len, 5000);
}


void PRINTF16(char str[50], uint16_t val){
  char msg[50];
  int msg_len;
  msg_len = sprintf(msg, str,val);
  HAL_UART_Transmit(&huart2, (uint8_t *)msg, msg_len, 5000);
}


void PRINTF32(char str[50], uint32_t val){
  char msg[50];
  int msg_len;
  msg_len = sprintf(msg, str,val);
  HAL_UART_Transmit(&huart2, (uint8_t *)msg, msg_len, 5000);
}


void PRINTFchar(char str[50], char lettere[20]){
	  char msg[50];
	  int msg_len;
	  msg_len = sprintf(msg, str,lettere);
	  HAL_UART_Transmit(&huart2, (uint8_t *)msg, msg_len, 5000);
}


