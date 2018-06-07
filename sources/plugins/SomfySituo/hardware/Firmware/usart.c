#include <xc.h>
#include <stdio.h>
#include "usart.h"

inline char itoc (int i)
{
    return i + '0';
}

void putch(unsigned char byte) {
    /* output one byte */
    while(!TXIF)    /* set when register is empty */
        continue;
    TXREG = byte;
}

void print_tx(const char* str, unsigned int len) {
    /* output one byte */
    putch(STX);
    for (int i=0; i<len; i++)
        putch(str[i]);
    putch(ETX);
}

void printc_tx(const char c) {
    /* output one byte */
    putch(STX);
    putch(c);
    putch(ETX);
}

void printci_tx(const char c, unsigned int i) {
    /* output one byte */
    putch(STX);
    putch(c);
    putch(itoc(i));
    putch(ETX);
}

unsigned char getch() {
    /* retrieve one byte */
    while(!RCIF)    /* set when register is not empty */
        continue;
    return RCREG;
}

/*unsigned char getche(void) {
    unsigned char c;
    putch(c = getch());
    return c;
}*/