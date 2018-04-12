/* 
 * File:   usart.h
 * Author: Henry Chung
 *
 */

#ifndef _SERIAL_H_
#define _SERIAL_H_

// config communications here
#define BAUD 9600
#define FREQUENCY 4000000L

// refer to data sheet to set these. Typical settings are below
#define MULTIPLIER 16UL // muiltiplier is typically 64UL, 16UL or 4UL
#define HIGH_SPEED 1 // refer to BRGH in datasheet to determine if you need high speed

// not common
#define NINE 0     /* Use 9bit communication? FALSE=8bit */

// Set TX & RX pins. Below are for 16f688; update for your chip
#define TX_PIN                RC4
#define RX_PIN                RC5

#define STX                   0x02
#define ETX                   0x03

/*
 * The rest is calculated, so no need to edit below this line
 */

#define DIVIDER ((int)(FREQUENCY/(MULTIPLIER * BAUD) -1))

#if NINE == 1
#define NINE_BITS 0x40
#else
#define NINE_BITS 0
#endif

#if HIGH_SPEED == 1
#define SPEED 0x4
#else
#define SPEED 0
#endif

// basic settings to enable serial
// RCSTA is 0x90: 10010000 (SPEN RX9 SREN CREN ADEN FERR OERR RX9D)
// TXSTA is 0x20: 00100000 (CSRC TX9 TXEN SYNC - BRGH TRMT TX9D)
#define RCSTA_DEFAULT 0x90
#define TXSTA_DEFAULT 0x20

/* Serial initialization */
#define init_comms()\
    RX_PIN = 1;    \
    TX_PIN = 1;          \
    SPBRG = DIVIDER;         \
    RCSTA = (NINE_BITS|RCSTA_DEFAULT);    \
    TXSTA = (SPEED|NINE_BITS|TXSTA_DEFAULT)

unsigned char getch(void);      // Get a character from the com
//unsigned char getche(void);   // get a char and send echo of the received char
void putch(unsigned char);      // Send a character to the com
void printc_tx(const char c);   // Send a character to the com with the STX ETX char
void printci_tx(const char c, unsigned int i);      // Send a character and an integer to the com with the STX ETX char
void print_tx(const char* str, unsigned int len);   // Send a string to the com with the STX ETX char

#endif