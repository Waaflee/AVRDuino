#include "interrupts.h"

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
    *mask |= _BV(pinA[pin - 14]);
  } else {
    *mask |= _BV(pinD[pin]);
  }
}

void setINT(uint8_t pin, enum EI_TRIGGER trigger) {
  EICRA |= trigger;
  EIMSK |= _BV(pin == 2 ? INT0 : INT1);
}
