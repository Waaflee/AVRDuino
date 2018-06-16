#pragma once

#include <string.h>
#include <avr/interrupt.h>
#define FALLING 0
#define RISING 1

enum prescaler {
  x1 = _BV(CS00),
  x8 = _BV(CS01),
  x64 = _BV(CS00) | _BV(CS01),
  x256 = _BV(CS02),
  x1024 = _BV(CS02) | _BV(CS00),
};

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////TIMER0/////////////////////////////////////



void setTimer0(enum prescaler PS);
void setTimer0PS(enum prescaler ps);


////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////TIMER1/////////////////////////////////////

enum t1_interrupts {
  input_capture = _BV(ICIE1),
  match_B = _BV(OCIE1B),
  match_A = _BV(OCIE1A),
  overflow = _BV(TOIE1)
};

struct timer1 {
  enum prescaler prescaler;
  enum t1_interrupts interrupts;
  uint8_t inputFlank;
  uint8_t inputNoiseCancelling;
  void (*InputCaptureHandler)();
  void (*MAHandler)();
  void (*MBHandler)();
  void (*OverflowHandler)();
  void (*regist)(struct timer1 *t1);
  void (*disable)();
};
struct timer1 newTimer1(void);
void Regist1 (struct timer1 *t1);

void InputCaptureHandler(void);
void T1_MAHandler(void);
void T1_MBHandler(void);
void T1_OverflowHandler(void);

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////TIMER2/////////////////////////////////////
