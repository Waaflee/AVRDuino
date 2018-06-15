#pragma once

#include <avr/interrupt.h>
#include "core.h"

enum EI_TRIGGER
{
    LOW_LEVEL = _BV(ISC00),
    ON_CHANGE = _BV(ISC01),
    FALLLING_FLANK = _BV(ISC10),
    RISING_FLANK = _BV(ISC10)
};

void setPCInt(uint8_t pin);
void setINT(uint8_t pin, enum EI_TRIGGER trigger);
