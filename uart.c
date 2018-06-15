#include "uart.h"

void (*command_interpreter)(char *);

void UART_init(void (*f)(char *)) {
  UARTcount = 0;
  UBRR0 = F_CPU / 16 / BAUD - 1;
  UCSR0A = 0;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  UCSR0C = (1 << USBS0) | (3 << UCSZ00);
  UARTSetted = TRUE;
  command_interpreter = f;
}

int uecho(char c, FILE *stream) {
  while (!(UCSR0A & (1 << UDRE0))) {
  };
  UDR0 = c;
  return 0;
}

int uread(FILE *stream) {
  while (!(UCSR0A & (1 << RXC0))) {
  };
  return UDR0;
}

void UARTclear(void) {
  for (uint8_t i = 0; i < 15; i++) {
    memset(UARTData, 0, sizeof UARTData);
  }
  UARTcount = 0;
}

ISR(USART_RX_vect) {
  uData = uread(&uart_io);
  switch (uData) {
  case 'H':
    printf("%s %s\n", "command: ", UARTData);
    printf("\n%s", "Usage:\n");
    printf("%s\n",
           "rotateNSteps:\t r<motor(0-3)><direction(f|b)>:<int stepps>");
    printf("%s\n", "position:\t p<motor(0-3)><rel or abs (r|a)>:<int where>");
    printf("%s\n", "setSpeed:\t s<motor(0-3)>::<int RPM>");
    printf("%s\n", "stop:\t\t b<motor(0-3)>");
    printf("%s\n", "where:\t\t w<motor(0-3)>");
    printf("%s\n", "to run command append 'C' or ';'");
    printf("%s\n", "to clear command put B");
    break;
  case 'B':
    printf("%s %s\n", "command: ", UARTData);
    UARTclear();
    break;
  case '\r':
    command_interpreter(UARTData);
    printf("%s %s\n", "command: ", UARTData);
    UARTclear();
    break;
  case '\n':
    command_interpreter(UARTData);
    printf("%s %s\n", "command: ", UARTData);
    UARTclear();
    break;
  case ';':
    command_interpreter(UARTData);
    printf("%s %s\n", "command: ", UARTData);
    UARTclear();
    break;
  default:
    UARTData[UARTcount++] = uData;
    break;
  }
}
