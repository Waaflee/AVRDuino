#include "timers.h"

void setTimer0(enum t0_prescaler PS) {
  // Set timer to click at F_CPU / PS.
  TCCR0B |= PS;
  // Enable timer overflow interrupts.
  TIMSK0 |= _BV(TOIE0);
  // Reset timer/counter zero.
  TCNT0 = 0;
  // Enable interrupts.
  sei();
};

void setTimer0PS(enum t0_prescaler ps) { TCCR0B = ps; };
