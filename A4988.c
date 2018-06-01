#include "A4988.h"
// #define MANSET
// #define SETPOL

#ifdef MANSET
void setPololu(pololu *drive, int dir, int step, int enable, int MS1, int MS2,
               int MS3, double degrees_per_step, int RPM) {
  drive->dir = dir;
  drive->step = step;
  drive->enable = enable;
  drive->MS1 = MS1;
  drive->MS2 = MS2;
  drive->MS3 = MS3;
  drive->degrees_per_step = degrees_per_step;
  drive->RPM = RPM;
  setPin(dir, OUTPUT);
  setPin(step, OUTPUT);
  setPin(enable, OUTPUT);
  setPin(MS1, OUTPUT);
  setPin(MS2, OUTPUT);
  setPin(MS3, OUTPUT);
}

A4988 newPololu(int dir, int step, int enable, int MS1, int MS2, int MS3,
                double degrees_per_step, int RPM) {
  pololu drive;
  drive.dir = dir;
  drive.step = step;
  drive.enable = enable;
  drive.MS1 = MS1;
  drive.MS2 = MS2;
  drive.MS3 = MS3;
  drive.degrees_per_step = degrees_per_step;
  drive.RPM = RPM;
  setPin(dir, OUTPUT);
  setPin(step, OUTPUT);
  setPin(enable, OUTPUT);
  setPin(MS1, OUTPUT);
  setPin(MS2, OUTPUT);
  setPin(MS3, OUTPUT);
  return drive;
}
#endif

#ifdef SETPOL
void setPololuFA(pololu *drive, DriveArray array) {
  drive->dir = (int)array[0];
  drive->step = (int)array[1];
  drive->enable = (int)array[2];
  drive->MS1 = (int)array[3];
  drive->MS2 = (int)array[4];
  drive->MS3 = (int)array[5];
  drive->degrees_per_step = array[6];
  drive->RPM = (int)array[7];

  for (uint8_t i = 0; i < 6; i++) {
    setPin((int)array[i], OUTPUT);
  }
}
#endif

A4988 newPololuFA(DriveArray array) {
  pololu drive;
  memset(&drive, 0, sizeof drive);
  drive.dir = (int)array[0];
  drive.step = (int)array[1];
  drive.enable = (int)array[2];
  drive.MS1 = (int)array[3];
  drive.MS2 = (int)array[4];
  drive.MS3 = (int)array[5];
  drive.degrees_per_step = array[6];
  drive.PPV = (int)(360 / array[6]);
  drive.RPM = (int)array[7];

  for (uint8_t i = 0; i < 3; i++) {
    setPin((int)array[i], OUTPUT);
  }

  return drive;
}

void setSpeed(int speed, STEPPER *drive) { drive->motor->RPM = speed; };

void rotateNSteps(int n, STEPPER *drive, int dir) {
  drive->motor->stepps = n;
  int accel = n / 4 > 100 ? 100 : n / 4;
  drive->motor->accelStepps[1] = n - accel;
  drive->motor->accelStepps[0] = accel;
  pinOn(drive->motor->enable);
  drive->enabled = TRUE;
  if (dir) {
    drive->motor->direction = FORWARD;
    pinOn(drive->motor->dir);
  } else {
    drive->motor->direction = BACKWARD;
    pinOff(drive->motor->dir);
  }
}

void goToabs(int position, STEPPER *drive) {
  int actual = drive->motor->location;
  int diff = actual - position;
  rotateNSteps(abs(diff), drive, diff < 0 ? FORWARD : BACKWARD);
}
void goTorel(int percentage, STEPPER *drive) {
  int position = ((float)percentage / 100) * drive->motor->MaxSteps;
  goToabs(position, drive);
}

void stopPololu(STEPPER *drive) {
  if (drivesInit.onSetup) {
    drivesInit.setupCount[drive->ID]++;
    if (drivesInit.setupCount[drive->ID] == 2) {
      drive->motor->MaxSteps = INIT_STEPPS - drive->motor->stepps;
      drivesInit.setted++;
      if (drivesInit.setted == NUM_STEPPERS) {
        drivesInit.onSetup = FALSE;
        if (UARTSetted) {
          for (uint8_t i = 0; i < NUM_STEPPERS; i++) {
            printf("PAP[%d] %s %d\n", PAParray[i]->ID, "MaxSteps:\t",
                   PAParray[i]->motor->MaxSteps);
          }
        }
      }
    } else {
      rotateNSteps(INIT_STEPPS, drive, !drive->motor->direction);
    }
  } else {
    drive->motor->stepps = 0;
    pinOff(drive->motor->enable);
    drive->enabled = FALSE;
  }
}

void raceEnd(uint8_t drive, uint8_t which) {
  PAParray[drive]->motor->location =
      which ? PAParray[drive]->motor->MaxSteps : 0;
  stopPololu(PAParray[drive]);
  if (!drivesInit.onSetup) {
    /* code */
    setSpeed(1, PAParray[drive]);
    rotateNSteps(4, PAParray[drive], !PAParray[drive]->motor->direction);
  }
}
void PAPsInit(uint8_t speed) {
  drivesInit.onSetup = TRUE;
  for (uint8_t i = 0; i < NUM_STEPPERS; i++) {
    setSpeed(speed, PAParray[i]);
    rotateNSteps(INIT_STEPPS, PAParray[i], FORWARD);
  }
}
