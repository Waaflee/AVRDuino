#include "uart.h"

void UART_init(void) {
  UBRR0 = F_CPU / 16 / BAUD - 1;
  UCSR0A = 0;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  UCSR0C = (1 << USBS0) | (3 << UCSZ00);
  UARTSetted = TRUE;
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

void UARTclear(void) {
  for (uint8_t i = 0; i < 15; i++) {
    memset(UARTData, 0, sizeof UARTData);
  }
  UARTcount = 0;
}

void checkData(char data[]) {
  char temp[15];
  int rotation;
  uint8_t direction;
  int speed;
  int coord;
  // printf("%s %s\n", "data: ", UARTData);
  switch (data[0]) {
  case 'r':
    direction = data[2] == 'f' ? FORWARD : BACKWARD;
    for (uint8_t i = 4; i < UARTcount; i++) {
      temp[i - 4] = data[i];
    }
    rotation = atoi(temp);
    rotateNSteps(rotation, PAParray[atoi(&data[1])], direction);
    break;
  case 's':
    for (uint8_t i = 4; i < UARTcount; i++) {
      temp[i - 4] = data[i];
    }
    speed = atoi(temp);
    setSpeed(speed, PAParray[atoi(&data[1])]);
    break;
  case 'b':
    stopPololu(PAParray[atoi(&data[1])]);
    break;
  case 'w':
    printf("PAP[%s] located in: %d\n", &data[1],
           PAParray[atoi(&data[1])]->motor->location);
    break;
  case 'p':
    for (uint8_t i = 4; i < UARTcount; i++) {
      temp[i - 4] = data[i];
    }
    coord = atoi(temp);
    switch (data[2]) {
    case 'r':
      goTorel(coord, PAParray[atoi(&data[1])]);
      break;
    case 'a':
      goToabs(coord, PAParray[atoi(&data[1])]);
      break;
    }
    break;
  }
}

ISR(USART_RX_vect) {
  uData = uread(&uart_io);
  printf("%s %c\n", "Recibido: ", uData);
  switch (uData) {
  case 'B':
    UARTclear();
    break;
  case 'C':
    checkData(UARTData);
    UARTclear();
    break;
  case ';':
    checkData(UARTData);
    UARTclear();
    break;
  case 'H':
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
  default:
    UARTData[UARTcount++] = uData;
  }
  printf("%s %s\n", "data: ", UARTData);
}
