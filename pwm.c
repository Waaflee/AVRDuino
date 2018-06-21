#include "pwm.h"

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////TIMER1/////////////////////////////////////

void registPWM1(struct pwm1 *pwm) {
  if (!TCCR1B) {
    TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
    TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS01);
  } else {
    uint8_t OCA = pwm->oca ? pwm->oca : proportionalA;
    uint8_t OCB = pwm->ocb ? pwm->ocb : proportionalB;
    uint8_t wgma;
    uint8_t wgmb;

    if (!pwm->waveform) {
      pwm->waveform = fast;
    };
    switch (pwm->waveform) {
    case fast:
      wgma = _BV(WGM11);
      wgmb = _BV(WGM13) | _BV(WGM12);
      break;
    case phaseCorrect:
      wgma = _BV(WGM11);
      wgmb = _BV(WGM13);
      break;
    default:
      break;
    }
  }
  pwm->dutyA = duty_1A;
  pwm->dutyB = duty_1B;
  pwm->freq = freq;
  // pwm->freq(1);
};

struct pwm1 newPWM1(void) {
  struct pwm1 pwm;
  memset(&pwm, 0, sizeof(struct pwm1));
  pwm.regist = registPWM1;
  return pwm;
}

uint16_t duty_1A(uint16_t ton) {
  if (ton == NaN) {
    return OCR1A;
  }
  OCR1A = ton;
  return 0;
};
uint16_t duty_1B(uint16_t ton) {
  if (ton == NaN) {
    return OCR1B;
  }
  OCR1B = ton;
  return 0;
};

uint16_t freq(uint16_t Khz) {
  if (Khz == NaN) {
    return ICR1;
  }
  ICR1 = Khz;
  // uint16_t factor;
  // uint8_t PRESCALER = (TCCR1B & (_BV(CS01) | _BV(CS00) | _BV(CS02)));
  // switch (PRESCALER) {
  // case (_BV(CS00)):
  //   factor = 16000;
  //   break;
  // case (_BV(CS01)):
  //   factor = 2000;
  //   break;
  // case (_BV(CS00) | _BV(CS01)):
  //   factor = 250;
  //   break;
  // case (_BV(CS02)):
  //   factor = 62;
  //   break;
  // case (_BV(CS02) | _BV(CS00)):
  //   factor = 16;
  //   break;
  // default:
  //   factor = 2000;
  //   break;
  // }
  // ICR1 = (uint16_t)((float)(float)factor / (float)Khz);
  return 0;
};
