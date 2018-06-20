#include "A4988.h"

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
};

void setSpeed(int speed, STEPPER *drive) { drive->motor->RPM = speed; };

void rotateNSteps(int n, STEPPER *drive, int dir) {
  drive->motor->stepps = n;
  int accel = n / 4 > 100 ? 100 : n / 4;
  drive->motor->accelStepps[1] = n - accel;
  drive->motor->accelStepps[0] = accel;
  pinOff(drive->motor->enable);
  drive->enabled = TRUE;
  if (dir) {
    drive->motor->direction = FORWARD;
    pinOn(drive->motor->dir);
  } else {
    drive->motor->direction = BACKWARD;
    pinOff(drive->motor->dir);
  }
};

void goToabs(int position, STEPPER *drive) {
  int actual = drive->motor->location;
  int diff = actual - position;
  rotateNSteps(abs(diff), drive, diff < 0 ? FORWARD : BACKWARD);
};

void goTorel(int percentage, STEPPER *drive) {
  int position = ((float)percentage / 100) * drive->motor->MaxSteps;
  goToabs(position, drive);
};

void stopPololu(STEPPER *drive) {
  if (drivesInit.onSetup) {
    drivesInit.setupCount[drive->ID]++;
    if (drivesInit.setupCount[drive->ID] == 2) {
      drive->motor->MaxSteps = INIT_STEPPS - drive->motor->stepps;
      drivesInit.setted++;
      if (drivesInit.setted == NUM_STEPPERS) {
        drivesInit.onSetup = FALSE;
#ifdef UART_DEBBUG
#include "uart.h"
        if (UARTSetted) {
          for (uint8_t i = 0; i < NUM_STEPPERS; i++) {
            printf("PAP[%d] MaxSteps:\t %d\n", PAParray[i]->ID,
                   PAParray[i]->motor->MaxSteps);
          }
        }
#endif
      }
    } else {
      rotateNSteps(INIT_STEPPS, drive, !drive->motor->direction);
    }
  } else {
    brake(drive);
  }
};

void brake(STEPPER *drive) {
  drive->motor->stepps = 0;
  pinOn(drive->motor->enable);
  drive->enabled = FALSE;
};

void raceEnd(uint8_t drive, uint8_t which) {
  PAParray[drive]->motor->location =
      which ? PAParray[drive]->motor->MaxSteps : 0;
  stopPololu(PAParray[drive]);
  if (!drivesInit.onSetup) {
    setSpeed(1, PAParray[drive]);
    rotateNSteps(4, PAParray[drive], !PAParray[drive]->motor->direction);
  }
};

void PAPsInit(uint8_t speed) {
  memset(&drivesInit, 0, sizeof(drivesInit));
  drivesInit.onSetup = TRUE;
  for (uint8_t i = 0; i < NUM_STEPPERS; i++) {
    setSpeed(speed, PAParray[i]);
    rotateNSteps(INIT_STEPPS, PAParray[i], FORWARD);
  }
};

#ifdef POLOLU_TIMER0_OVERFLOW

volatile int RPM;
volatile int count[NUM_STEPPERS];
volatile long int delay;

// Timer0 overflow interruption, checks enabled steppers setep's left and
// moves them accordingly until there is no one left.
ISR(TIMER0_OVF_vect, ISR_NOBLOCK) {
  // increases each stepper's count assigned variable
  for (uint8_t i = 0; i < NUM_STEPPERS; i++) {
    count[i]++;
    // check if current stepper is enabled
    if (PAParray[i]->enabled) {
      // check if current stepper has any steps left (checking could be merged
      // with above's conditional but I think this way is more readable).
      if (PAParray[i]->motor->stepps > 0) {
        // Calculates amount of overflows until next stepp accordingly to
        // current stepper's RPM
        if (PAParray[i]->motor->stepps > PAParray[i]->motor->accelStepps[1]) {

          RPM = PAParray[i]->motor->RPM -
                (1000 * PAParray[i]->motor->RPM /
                 PAParray[i]->motor->accelStepps[0]) *
                    (PAParray[i]->motor->stepps -
                     PAParray[i]->motor->accelStepps[1]) /
                    1000;

        } else if ((PAParray[i]->motor->stepps <=
                    PAParray[i]->motor->accelStepps[1]) &&
                   (PAParray[i]->motor->stepps >=
                    PAParray[i]->motor->accelStepps[0])) {

          RPM = PAParray[i]->motor->RPM;

        } else if (PAParray[i]->motor->stepps <
                   PAParray[i]->motor->accelStepps[0]) {

          RPM = ((1000 * PAParray[i]->motor->RPM /
                  PAParray[i]->motor->accelStepps[0]) *
                 PAParray[i]->motor->stepps) /
                1000;
        }

        delay = (60 * (7812 / PAParray[i]->motor->PPV)) / RPM;
        // in order to emulate a square shaped wave, the stepper's step pin will
        // turn on in the middle of the dealy and turn off again at it's end.
        if (count[i] >= delay / 2) {
          pinOn(PAParray[i]->motor->step);
          if (count[i] >= delay) {
            PAParray[i]->motor->stepps--;
            pinOff(PAParray[i]->motor->step);
            count[i] = 0;
            // Checks current direction and upgrades position accordingly
            if (PAParray[i]->motor->direction) {
              PAParray[i]->motor->location++;
            } else {
              PAParray[i]->motor->location--;
            }
            // if there are not stepps left the current stepper will be
            // disabled.
            if (PAParray[i]->motor->stepps == 0) {
              PAParray[i]->enabled = FALSE;
              pinOff(PAParray[i]->motor->enable);
            }
          }
        }
      }
    }
  }
};
#endif
