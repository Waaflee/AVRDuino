#include "core.h"

uint8_t pinIn[] = {PIND0, PIND1, PIND2, PIND3, PIND4, PIND5, PIND6,
                   PIND7, PINB0, PINB1, PINB2, PINB3, PINB4, PINB5};

int pinout[] = {0, 1, 2, 3, 4,    5,    6,    7,    0,    1,
                2, 3, 4, 5, DDC0, DDC1, DDC2, DDC3, DDC4, DDC5};
int pinD[] = {0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5};

int pinA[] = {PORTC0, PORTC1, PORTC2, PORTC3, PORTC4, PORTC5};

int *getPort(int pin) {
  if (pin <= 7) {
    return (int *)&PORTD;
  } else if (pin >= 14) {
    return (int *)&PORTC;
  } else {
    return (int *)&PORTB;
  }
}

int *getDDPort(int pin) {
  if (pin <= 7) {
    return (int *)&DDRD;
  } else if (pin >= 14) {
    return (int *)&DDRC;
  } else {
    return (int *)&DDRB;
  }
}

void setPin(int pin, int state) {
  int *port = getDDPort(pin);
  if (!state) {
    *port |= _BV(pinout[pin]);
  } else {
    *port &= ~_BV(pinout[pin]);
  }
}

void pinOn(int pin) {
  int *puerto = getPort(pin);
  *puerto |= _BV(pinD[pin]);
}

void pinOff(int pin) {
  int *puerto = getPort(pin);
  *puerto &= ~_BV(pinD[pin]);
}

void togglePin(int pin) {
  int *puerto = getPort(pin);
  *puerto ^= _BV(pinD[pin]);
}

int readDPin(int pin) {
  int *Port = pin <= 7 ? (int *)&PIND : (int *)&PINB;
  if (*Port & _BV(pinIn[pin])) {
    return 1;
  } else {
    return 0;
  }
}
