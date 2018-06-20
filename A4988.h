#pragma once

#define FORWARD 1
#define BACKWARD 0
#define START 0
#define END 1

#include "core.h"
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../variables.h"

typedef struct drives_init {
  uint8_t onSetup;
  uint8_t setted;
  uint8_t setupCount[NUM_STEPPERS];
} drives_Init;

drives_Init drivesInit;

//A4988 driver struct and function declarations, you may use the custom pololu
//type should you find it more convenient.

typedef struct a4988 {
        int dir;
        int step;
        int enable;
        int MS1;
        int MS2;
        int MS3;
        double degrees_per_step;
        int PPV;
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

STEPPER *PAParray[NUM_STEPPERS];

//initializes pololu struct from a DriveArray array;
A4988 newPololuFA(DriveArray array);

//Changes selected a4988 instance derided RPMs
void setSpeed(int speed, STEPPER *drive);

//Command selected driver to move n amount of stepps in the desired directon
void rotateNSteps(int n, STEPPER *drive, int dir);

// AVIABLE AFTER PAPsInit() IS COMPLETED!!!!!!
// Commands drive to move to an absolute position in stepps on the aviable range
void goToabs(int position, STEPPER *drive);
// Commands drive to move to a relative position in % on the aviable range
void goTorel(int percentage, STEPPER *drive);

//Stops and disables selected drive.
void stopPololu(STEPPER *drive);
//Stops pololu no matter if homming
void brake(STEPPER *drive);
//Wrapper for stopPololu(), designed for race ends
void raceEnd(uint8_t drive, uint8_t which);
//Homing routine
void PAPsInit(uint8_t speed);
