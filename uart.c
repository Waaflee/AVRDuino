#include "uart.h"

void UART_init(void) {
  UBRR0 = F_CPU / 16 / BAUD - 1;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

int uecho(char c, FILE *stream) {
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = c;
  return 0;
}

int uread(FILE *stream) {
  while (!(UCSR0A & (1 << RXC0)))
    ;
  return UDR0;
}
