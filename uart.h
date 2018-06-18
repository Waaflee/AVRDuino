#pragma once

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../variables.h"
#define FORWARD 1
#define BACKWARD 0

char UARTData[15];
volatile char uData;
uint8_t UARTcount;

void UART_init(void (*f)(char *));
int uecho(char, FILE *stream);
int uread(FILE *stream);
extern FILE uart_io;
void UARTclear(void);
