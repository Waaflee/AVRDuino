#include "interrupts.h"

int pinesD[] = {0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5};

int pinesA[] = {PORTC0, PORTC1, PORTC2, PORTC3, PORTC4, PORTC5};

void setPCInt(uint8_t pin) {
  uint8_t ctrlReg;
  int *mask;
  if (pin >= 8 && pin <= 13) {
    ctrlReg = PCIE0;
    mask = (int *)&PCMSK0;
  } else if (pin >= 0 && pin <= 7) {
    ctrlReg = PCIE2;
    mask = (int *)&PCMSK2;
  } else if (pin >= 14 && pin <= 20) {
    ctrlReg = PCIE1;
    mask = (int *)&PCMSK1;
  }
  PCICR |= _BV(ctrlReg);
  if (pin >= 14) {
    *mask |= _BV(pinesA[pin - 14]);
  } else {
    *mask |= _BV(pinesD[pin]);
  }
}

void setINT(uint8_t pin, enum EI_TRIGGER trigger, void (*f)()) {
  EICRA |= trigger;
  EIMSK |= _BV(pin == 2 ? INT0 : INT1);
  if (pin == 2) {
    INT0_handler = f;
  } else {
    INT1_handler = f;
  }
}

ISR(INT0_vect) { INT0_handler(); }
ISR(INT1_vect) { INT1_handler(); }
