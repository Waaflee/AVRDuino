#pragma once

#define BAUD 9600
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

void UART_init(void);
int uecho(char,FILE *stream);
int uread(FILE *stream);
