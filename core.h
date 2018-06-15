#pragma once
#define OUTPUT 0
#define INPUT 1
#define FALSE 0
#define TRUE 1

#include <avr/io.h>
#define A(x) (x + 14)



int *getPort(int pin);
int *getDDPort(int pin);
void setPin(int pin, int state);
void pinOn(int pin);
void pinOff(int pin);
void togglePin(int pin);

int readDPin(int pin);
