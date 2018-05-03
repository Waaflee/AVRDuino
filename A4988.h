#pragma once
#define FORWARD 1
#define BACKWARD 0
#define START 0
#define END 1
#include "core.h"
#include "A4988_interrups.h"
#include "uart.h"
#include "../../variables.h"

typedef struct drives_init {
  uint8_t onSetup;
  uint8_t setted;
  unsigned int setupCount[NUM_STEPPERS];

} drives_Init;
drives_Init drivesInit;

// #define MANSET
// #define SETPOL

//A4988 driver struct and function declarations, you may use the custom pololu
//type if you find it more convenient.

typedef struct a4988 {
        int dir;
        int step;
        int enable;
        int MS1;
        int MS2;
        int MS3;
        double degrees_per_step;
        int RPM;
        int stepps;
        int accelStepps[2];
        int location;
        int MaxSteps;
        uint8_t direction;
} A4988;

typedef A4988 pololu;
typedef double DriveArray[8];

typedef struct stepper {
  unsigned short int ID;
  unsigned short int enabled;
  A4988 *motor;
}STEPPER;

volatile int count[NUM_STEPPERS];
volatile int delay;
STEPPER *PAParray[NUM_STEPPERS];

#ifdef MANSET
  //set existing A4988 struct manually or from an existing DriveArray array.
  void setPololu(pololu *drive, int dir, int step, int enable, int MS1, int MS2,
                 int MS3, double degrees_per_step, int RPM);
  //initializes pololu struct manually.
  A4988 newPololu(int dir, int step, int enable, int MS1, int MS2, int MS3, double degrees_per_step, int RPM);
  void setPololuFA(pololu *drive, DriveArray array);
#endif

//initializes pololu struct from a DriveArray array;
A4988 newPololuFA(DriveArray array);

void setSpeed(int speed, STEPPER *drive);
void rotateNSteps(int n, STEPPER *drive, int dir);

void goToabs(int position, STEPPER *drive);
void goTorel(int percentage, STEPPER *drive);

void stopPololu(STEPPER *drive);
void raceEnd(uint8_t drive, uint8_t which);

void PAPsInit(void);
