#pragma once

#define BAUD 9600
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "core.h"
#include "A4988.h"
#define FORWARD 1
#define BACKWARD 0

char UARTData[15];
volatile char uData;
uint8_t UARTcount;

void UART_init(void);
int uecho(char,FILE *stream);
int uread(FILE *stream);
extern FILE uart_io;
void checkData(char data[]);
void UARTclear(void);
