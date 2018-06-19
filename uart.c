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
  memset(UARTData, 0, sizeof UARTData);
  UARTcount = 0;
}

ISR(USART_RX_vect) {
  uData = uread(&uart_io);
  switch (uData) {
  case '\r':
    command_interpreter(UARTData);
    UARTclear();
    break;
  case '\n':
    command_interpreter(UARTData);
    UARTclear();
    break;
  default:
    UARTData[UARTcount++] = uData;
    break;
  }
}
