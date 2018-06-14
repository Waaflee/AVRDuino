# AVRDuino
Simple AVR lib for arduino that wraps registers operations in Arduino-like functions

This is mean to be used as a submodule for proyects that require this library

## TODO

- [ ] **Readeable Documentation!!**
- [x] core: pin on, off, toggle, read (digital)
- [ ] core: pin on, off, toggle, read (analog)
- [x] core: PC & External Interrupts
- [ ] core: implemment interrupt declarations as `attachinterrupt(pin, func*)`
- [ ] interrupts: migrate interruption declaration and handling from core to this
- [x] A4988 control
- [x] UART: setup & command Interpreter
- [ ] Kick out command interpretter from uart.c
- [ ] Create widely reachable command interpreter so as to be able to expose an
API to any comunication interface
- [ ] Timers setup (struct argumented functions)
- [ ] PWM might be merged with TIMERS
