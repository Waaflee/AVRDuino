#include "A4988_interrupts.h"

#ifdef POLOLU

volatile int RPM;
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
}
#endif
