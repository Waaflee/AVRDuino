#pragma once
#define OUTPUT 0
#define INPUT 1
#define FALSE 0
#define TRUE 1

#include <avr/io.h>
#include <avr/interrupt.h>
#define A(x) (x + 14)

enum EI_TRIGGER
{
    LOW_LEVEL = _BV(ISC00),
    ON_CHANGE = _BV(ISC01),
    FALLLING_FLANK = _BV(ISC10),
    RISING_FLANK = _BV(ISC10)
};

int *getPort(int pin);
int *getDDPort(int pin);
void setPin(int pin, int state);
void pinOn(int pin);
void pinOff(int pin);
void togglePin(int pin);

int readDPin(int pin);

void setPCInt(uint8_t pin);
void setINT(uint8_t pin, enum EI_TRIGGER trigger);
