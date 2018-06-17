#pragma once

#define khz
#define NaN 65535

#define pt100(x) (uint16_t)(((float)x / 100.0) * NaN)
#define pt1000(x) (uint16_t)(((float)x / 1000.0) * NaN)
#define pt10(x) (((uint16_t)x * 64) - ((x==1024) ? 1 : 0))
#define to100(x) (uint16_t)( ((float)x / (float)NaN) * 100.0 + 1)
#define to10(x) ((uint16_t)x / 64)


#include <avr/io.h>
#include <string.h>
////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////TIMER1/////////////////////////////////////

//timer 1 pwm WGM
enum t1_waveforms {
        fast, phaseCorrect
};

enum t1A_modes {
        unplugged = 0,
        toggleA = _BV(COM1A0),
        proportionalA = _BV(COM1A1),
        inverseA = _BV(COM1A1) | _BV(COM1A0)
};

enum t1B_modes {
        toggleB = _BV(COM1B0),
        proportionalB = _BV(COM1B1),
        inverseB = _BV(COM1B1) | _BV(COM1B0)
};

struct pwm1 {
        enum t1_waveforms waveform;
        enum t1A_modes oca;
        enum t1B_modes ocb;
        uint16_t (*dutyA)(uint16_t);
        uint16_t (*dutyB)(uint16_t);
        uint16_t (*freq)(uint16_t);
        void (*regist)(struct pwm1 *pwm);
};

struct pwm1 newPWM1(void);
void registPWM1 (struct pwm1 *pwm);
uint16_t duty_1A(uint16_t ton);
uint16_t duty_1B(uint16_t ton);
uint16_t freq(uint16_t Khz);

/////////////////////////////////////TIMER1/////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
