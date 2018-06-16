#include "timers.h"

void setTimer0(enum prescaler PS) {
  // Set timer to click at F_CPU / PS.
  TCCR0B |= PS;
  // Enable timer overflow interrupts.
  TIMSK0 |= _BV(TOIE0);
  // Reset timer/counter zero.
  TCNT0 = 0;
  // Enable interrupts.
  sei();
};

void setTimer0PS(enum prescaler ps) { TCCR0B = ps; };

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////TIMER1/////////////////////////////////////

// Sets timer 1 with config given by a timer1 struct

void Regist1(struct timer1 *t1) {

  TCCR1B = t1->prescaler | t1->interrupts | t1->inputFlank |
           t1->inputNoiseCancelling;
  TCNT1 = 0;
};

void disable() { TCCR1B = 0; };

struct timer1 newTimer1(void) {
  struct timer1 timer;
  memset(&timer, 0, sizeof(struct timer1));
  timer.regist = Regist1;
  timer.disable = disable;
  return timer;
}

/////////////////////////////////////TIMER1/////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
