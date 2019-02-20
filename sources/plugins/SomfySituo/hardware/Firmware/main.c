#pragma config FOSC = INTOSCIO //INTOSCCLK // Oscillator Selection bits (INTOSC oscillator) 


#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Detect (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)


#define _XTAL_FREQ 4000000
//INCLUDES
#include <xc.h>

#include <pic16f688.h>
#include <stdio.h>
//#include <stdlib.h>
#include <ctype.h>
//#include <string.h>
#include "usart.h"

/* Configuration des entrees-sorties (voir plus haut pour le mapping) */
#define ALL_INPUTS            0xFF
#define ALL_OUTPUTS           0x00
/* - Port A */
#define PORTA_DIR             0xDF //0xCF //0xD7 alive led en entrée temporairement
#define PIN_IN_LED_1          RA0
#define PIN_IN_LED_2          RA1
#define PIN_IN_LED_3          RA2
#define PIN_IN_LED_4          RA3
#define PIN_OUT_ALIVE_LED     RA4
#define PIN_OUT_SW_PROG       RA5

/* - Port B */
#define PORTB_DIR             ALL_INPUTS

/* - Port C */
#define PORTC_DIR             0xE0  //btn in output mode
#define PORTC_BTN_RELEASED    0xE7  //btn in input mode
#define PIN_OUT_SW_TOP        RC0
#define PIN_OUT_SW_BOTTOM     RC1
#define PIN_OUT_SW_MY         RC2
#define PIN_OUT_SW_CHANNEL    RC3
#define PIN_TX                RC4
#define PIN_RX                RC5

/* Octet non signe */
typedef char UBYTE;
typedef enum
{
    FALSE = 0,
    TRUE = 1
}BOOL;


/* Version */
#define STR_VERSION                       "v01.01"
#define STR_FIRMWARE                      ("sSituo5-" STR_VERSION)  //the first 's' is for the response of the command
#define STR_FIRMWARE_LEN                  14
#define NUMBER_OF_CHANNEL                 5

/* Trame RS232 */
#define RS232_LEN_HEADER_FOOTER           2    /* Longueur caracteres de contrôles */
#define RS232_LEN_CMD                     1    /* Longueur commande */
#define RS232_LEN_MIN_FRAME               (RS232_LEN_HEADER_FOOTER + RS232_LEN_CMD)

/* Index dans la trame */
#define RS232_IDX_CMD                     1
/* Commandes */
/*   - Status */
#define RS232_CMD_STATUS                  'S'
/*   - Config */
#define RS232_CMD_CONFIG                  'C'
#define RS232_CONFIG_DEBUG                'D'
/*   - Read */
#define RS232_CMD_READ                    'R'
/*   - Write */
#define RS232_CMD_WRITE                   'W'
/*   - Channel */
#define RS232_CMD_CHANNEL                 'L'
/*   - Somfy UP */
#define RS232_CMD_UP                      'U'
#define RS232_CMD_QUICK_UP                'u'
/*   - Somfy DOWN */
#define RS232_CMD_DOWN                    'D'
#define RS232_CMD_QUICK_DOWN              'd'
/*   - Somfy MY */
#define RS232_CMD_MY                      'M'
#define RS232_CMD_QUICK_MY                'm'
/*   - Somfy PROG */
#define RS232_CMD_PROG                    'P'
#define RS232_CMD_QUICK_PROG              'p'

/* Reponses */
#define RS232_REP_ERROR                   'E'
/* Longueur max de la trame = longueur de la trame la plus longue */
#define RS232_MAX_LEN_FRAME               (RS232_LEN_HEADER_FOOTER + RS232_LEN_CMD + 4)

#define DEFAULT_DELAY 250 //ms
#define FAST_DELAY 50    //ms

/* Codes d'erreur */
typedef enum
{
   ERR_UNKNOWN_COMMAND = 1,               /* Commande inconnue */
   ERR_FRAME_TOO_SMALL,                   /* Trame trop petite */
   ERR_FRAME_TOO_LONG,                    /* Trame trop longue */
   ERR_NO_ETX_FOUND,                      /* Marqueur de fin de trame non trouve */
   ERR_WRONG_DATAS,                       /* Donnees incorrectes */
   ERR_WRONG_DATAS_LENGTH,                /* Longueur de donnees incorrecte (1 a 16 autorise) */
} T_ERROR_CODES;

typedef enum
{
    MSG_ERROR=0,
    MSG_ANY,
    MSG_START,
    MSG_ACQUIRE,
    MSG_COMPLETE
} T_MESSAGE_STATE;

typedef enum
{
    NO_DEBUG=0,
    DEBUG_LIGHT,
    DEBUG_MORE,
    DEBUG_ALL
} T_DEBUG_MODE;

// Define scaler
#define CTR_LED_STARTUP      2     
#define CTR_LED_RUNTIME      4
#define CTR_LED_ERROR        2

/* Variables globales */
/* Trame RS232 */
static UBYTE Rs232Frame[RS232_MAX_LEN_FRAME];
/* Longueur de la trame */
static UBYTE LenRs232 = 0;
/* Indicateur de reception d'une trame */
static T_MESSAGE_STATE NewTs232Frame = MSG_ANY;
static T_ERROR_CODES Err_Code = ERR_UNKNOWN_COMMAND;
static int tCount = 3;
static int scaler = 4; //We put prescaler to 256.The scaler will make scaler x prescaler
static int scaleCount = 4; //We put prescaler to 256.The scaler will make scaler x prescaler
static unsigned int channel = 0;
static unsigned int lastActiveChannel = 0;

int flagRXFramingError = 0;
int flagRXOverrunError = 0;
volatile unsigned char RXIN = 3;
T_DEBUG_MODE DebugMode = NO_DEBUG;

inline int ctoi (char c)
{
    return c - '0';
}
inline int pow (const int a, const int b)
{
    int result = 1;
    if (b > 0)
        for (int i=1; i<=b; i++)
            result *= a;
    return result;
}

void delay_ms(int x)
{
    if (DebugMode == DEBUG_MORE) printci_tx('d',x);
    //x /= 2;  // for a reason, need to divide by 2 the value to obtain the desired delay (have to see around oscillator and frequencies))
    for (int i = 0; i < x; i++) 
    {
        __delay_ms(1);
    }
}

void fast_delay_ms(void)
{ 
    __delay_ms(FAST_DELAY);
}

/*void flashAliveLED (int toggleCount, int changeScaler)
{
    tCount = toggleCount;
    scaler = changeScaler;
    INTCONbits.T0IE = 1; // Enable interrupt on TMR0 overflow

}*/

void PushBtnUp (void)
{
    TRISC0 = 0; // Put the I/O in Output mode to be sure to have 0V in output
    PIN_OUT_SW_TOP = 0;       
}
void ReleaseBtnUp (void)
{
    TRISC0 = 1; // Put the I/O in Input mode to avoid having 5V in output
    PIN_OUT_SW_TOP = 1;
}

void PushBtnDown (void)
{
    TRISC1 = 0; // Put the I/O in Output mode to be sure to have 0V in output
    PIN_OUT_SW_BOTTOM = 0;       
}
void ReleaseBtnDown (void)
{
    TRISC1 = 1; // Put the I/O in Input mode to avoid having 5V in output
    PIN_OUT_SW_BOTTOM = 1;
}

void PushBtnMy (void)
{
    TRISC2 = 0; // Put the I/O in Output mode to be sure to have 0V in output
    PIN_OUT_SW_MY = 0;       
}
void ReleaseBtnMy (void)
{
    TRISC2 = 1; // Put the I/O in Input mode to avoid having 5V in output
    PIN_OUT_SW_MY = 1;
}

void PushBtnChannel (void)
{
    TRISC3 = 0; // Put the I/O in Output mode to be sure to have 0V in output
    PIN_OUT_SW_CHANNEL = 0;       
}
void ReleaseBtnChannel (void)
{
    TRISC3 = 1; // Put the I/O in Input mode to avoid having 5V in output
    PIN_OUT_SW_CHANNEL = 1;
}

void PushBtnProg (void)
{
    TRISA5 = 0; // Put the I/O in Output mode to be sure to have 0V in output
    PIN_OUT_SW_PROG = 0;       
}
void ReleaseBtnProg (void)
{
    TRISA5 = 1; // Put the I/O in Input mode to avoid having 5V in output
    PIN_OUT_SW_PROG = 1;
}
void ProcessReadChannel(void)
{
    channel = !PIN_IN_LED_1 + !PIN_IN_LED_2*2 + !PIN_IN_LED_3*3 + !PIN_IN_LED_4*4;
    if (channel == 10) channel = 5;
    if (channel > 0 && channel <= 5) lastActiveChannel = channel; // We don't store the fact that the LEDs switched off
    if (DebugMode != NO_DEBUG) {print_tx("RChan:",6);printci_tx('c',channel);}
    if (DebugMode == DEBUG_MORE) {print_tx("Pins:",5);printci_tx('1',!PIN_IN_LED_1);printci_tx('2',!PIN_IN_LED_2);printci_tx('3',!PIN_IN_LED_3);printci_tx('4',!PIN_IN_LED_4);}
}

void CommandChannel (unsigned long d)
{
    PushBtnChannel();
    delay_ms(d);
    ReleaseBtnChannel();
    delay_ms(d);
}


void CmdMultiChannel (unsigned int chanToActive, unsigned long delay)
{
    unsigned int nbChange = 0; // Number of changement we have to do to active the good channel
    ProcessReadChannel ();      // First get the LED's' state
    if (DebugMode != NO_DEBUG) {print_tx("MChan:",6);printci_tx('c',channel);printci_tx('l',lastActiveChannel);}
    if (lastActiveChannel == 0) // First call : we first need to wake up the LEDs to know which channel is the active one
    {                           // Should happen only once
        if (DebugMode != NO_DEBUG) print_tx("1stCall",7);
        CommandChannel(delay);
        ProcessReadChannel ();     // Save the change
        if (DebugMode != NO_DEBUG) {printci_tx('c',channel);printci_tx('l',lastActiveChannel);print_tx("/1stCall",8);}
    }
    
    // Get the number of time we need to change the channel to activate the good one
    if (chanToActive == lastActiveChannel) // We already have the good channel
        return; 
    else if (chanToActive < lastActiveChannel)
        nbChange = (NUMBER_OF_CHANNEL + chanToActive) - lastActiveChannel ;
    else if (chanToActive > lastActiveChannel)
        nbChange = chanToActive - lastActiveChannel;
    
    if (channel == 0) nbChange++; // We have to make a first call to light the LEDs
    
    if (DebugMode != NO_DEBUG) {print_tx("NbChange",8);printci_tx('=',nbChange);}
    // Change the channel
    for (int i=0; i < nbChange; i++)
    {
        CommandChannel(delay);
        delay_ms(delay);
    }
    ProcessReadChannel (); //marche pas
    if (DebugMode != NO_DEBUG)
    {
        printci_tx('c',channel);
        printci_tx('l',lastActiveChannel);
        print_tx("/MChan",6);
    }
}

void processCmdIN()
{
    unsigned long my_delay = DEFAULT_DELAY;
    unsigned int selected_channel = 0;
    if (LenRs232 > 1)
    {
        switch (Rs232Frame[0])
        {
            case RS232_CMD_STATUS :
            case RS232_CMD_READ :
            {
                NewTs232Frame = MSG_ERROR;
                Err_Code = ERR_FRAME_TOO_LONG;
                break;
            }
            case RS232_CMD_CONFIG :
            {
                if (LenRs232 > 3)
                {
                    NewTs232Frame = MSG_ERROR;
                    Err_Code = ERR_FRAME_TOO_LONG;
                }
                else if (LenRs232 < 2)
                {
                    NewTs232Frame = MSG_ERROR;
                    Err_Code = ERR_FRAME_TOO_SMALL;
                }
                else
                {
                    switch (Rs232Frame[1])
                    {
                        case RS232_CONFIG_DEBUG :
                        {
                            if (Rs232Frame[2] == '1')
                            {
                                DebugMode = DEBUG_LIGHT;
                                print_tx ("c-DebugON-", 10);
                            }
                            else if (Rs232Frame[2] == '0')
                            {
                                DebugMode = NO_DEBUG;
                                print_tx ("c-DebugOFF-", 11);
                            }
                            else if (Rs232Frame[2] == '2')
                            {
                                DebugMode = DEBUG_MORE;
                                print_tx ("c-Debug+ON-", 10);
                            }
                            else 
                            {
                                NewTs232Frame = MSG_ERROR;
                                Err_Code = ERR_WRONG_DATAS;
                            }
                            break;
                        }
                        default :
                            {
                                NewTs232Frame = MSG_ERROR;
                                Err_Code = ERR_WRONG_DATAS;
                                break;
                            }
                    }
                }       
                break;
            }
            case RS232_CMD_CHANNEL :
            case RS232_CMD_UP :
            case RS232_CMD_DOWN :
            case RS232_CMD_MY :
            case RS232_CMD_PROG :
            {
                 my_delay = 0;
                int lenDelay = LenRs232-1;
                if (DebugMode == DEBUG_MORE) print_tx("MyDelay",7);
                for (int i=0; i < lenDelay && NewTs232Frame != MSG_ERROR; i++)
                {
                    if (DebugMode == DEBUG_MORE) printci_tx('*',my_delay);
                    if (isdigit(Rs232Frame[i+1])==TRUE)
                    {
                        my_delay += (unsigned long) (ctoi(Rs232Frame[i+1])) * pow (10, lenDelay-i-1);
                    }
                    else 
                    {
                        NewTs232Frame = MSG_ERROR;
                        Err_Code = ERR_WRONG_DATAS;
                    }                
                }
                break;
            }
            
            case RS232_CMD_QUICK_UP :
            case RS232_CMD_QUICK_DOWN :
            case RS232_CMD_QUICK_MY :
            case RS232_CMD_QUICK_PROG :
            {
                if (LenRs232 > 2)
                {
                    NewTs232Frame = MSG_ERROR;
                    Err_Code = ERR_FRAME_TOO_LONG;
                }
                else if (!isdigit(Rs232Frame[1]))
                {
                    NewTs232Frame = MSG_ERROR;
                    Err_Code = ERR_WRONG_DATAS;
                }
                else
                {
                    selected_channel = (unsigned int) ctoi(Rs232Frame[1]);
                    my_delay = FAST_DELAY;
                }
                break;
            }
        }
        
    }
    if (NewTs232Frame != MSG_ERROR)
    {
        switch (Rs232Frame[0])
          {

            case RS232_CMD_STATUS :
             {
                print_tx(STR_FIRMWARE, STR_FIRMWARE_LEN );
                 break;
             }
             case RS232_CMD_CONFIG :
             {
                //Already treated
                break;
             }
            case RS232_CMD_READ :
             {
                printc_tx('r');
                break;
             }
            case RS232_CMD_WRITE :
             {
                printc_tx('w');
                break;
             }
            case RS232_CMD_CHANNEL :
             {
                CommandChannel(my_delay); //no ACK, it will be send by interruption
                ProcessReadChannel();
                break;
             }
            case RS232_CMD_QUICK_UP :
            {
                CmdMultiChannel (selected_channel, my_delay);
                PushBtnUp();
                fast_delay_ms();
                ReleaseBtnUp();
                printc_tx('u');
                break;
            }
            case RS232_CMD_UP :
            {
                PushBtnUp();
                delay_ms(my_delay);
                ReleaseBtnUp();
                printc_tx('U');
                break;
            }
            
            case RS232_CMD_QUICK_DOWN :
            {
                CmdMultiChannel (selected_channel, my_delay);
                PushBtnDown();
                fast_delay_ms();
                ReleaseBtnDown();
                printc_tx('d');
                break;
            }   
            case RS232_CMD_DOWN :
            {
                PushBtnDown();
                delay_ms(my_delay);
                ReleaseBtnDown();
                printc_tx('D');
                break;
            }
            case RS232_CMD_QUICK_MY :
            {
                CmdMultiChannel (selected_channel, my_delay);                
                PushBtnMy();
                fast_delay_ms();
                ReleaseBtnMy();
                printc_tx('m');
                break;
            }
            case RS232_CMD_MY :
            {
                PushBtnMy();
                delay_ms(my_delay);
                ReleaseBtnMy();
                printc_tx('M');
                break;
            }
            case RS232_CMD_QUICK_PROG :
            {
                CmdMultiChannel (selected_channel, my_delay);
                PushBtnProg();
                fast_delay_ms();
                ReleaseBtnProg();
                printc_tx('p');
                break;
            }
            case RS232_CMD_PROG :
            {
                // Programmation: push 2s to 5s button PROG on the individual remote control until the back and forth
                // the programmation mode stay for 10 min long
                // Select the channel for wich you want to add or withdraw the application
                // make a brief push on the PROG button on the Situo  until the back and forth
                // it's done.
                PushBtnProg();
                delay_ms(my_delay);
                ReleaseBtnProg();
                printc_tx('P');
                break;
            }
            default:
            {
                /* Commande inconnue */
                printci_tx('e', ERR_UNKNOWN_COMMAND);
                break;
            }
          }
         //flashAliveLED(3, CTR_LED_RUNTIME);  
    }
    //reinit of the message
    LenRs232 = 0;
    NewTs232Frame = MSG_ANY;
}

// Treat incomming message
void processRXIN(volatile unsigned char c_in)
{
    switch (c_in)
    {
        // STX Flag - Message begin
        case STX :
         {
            NewTs232Frame = MSG_START;
            LenRs232 = 0;
            break;
         }
        // ETX Flag - Message end
         case ETX :
         {
            if (NewTs232Frame == MSG_ACQUIRE)
                NewTs232Frame = MSG_COMPLETE;
            else 
            {
                NewTs232Frame = MSG_ERROR;
                Err_Code = ERR_FRAME_TOO_SMALL;
            }
            break;
         }
        // Message
        default :
        {
            if (NewTs232Frame == MSG_START ||
                    NewTs232Frame == MSG_ACQUIRE)
            {
                if (LenRs232 < RS232_MAX_LEN_FRAME)
                {
                    // Acquire the message
                    Rs232Frame[LenRs232] = c_in;
                    LenRs232 ++;
                    NewTs232Frame = MSG_ACQUIRE;
                }
                else if (LenRs232 >= RS232_MAX_LEN_FRAME)
                {
                    // Message is too long, it's not a good one
                    NewTs232Frame = MSG_ERROR;
                    Err_Code = ERR_FRAME_TOO_LONG;
                }
            }
            break;                
        }
    }
    
    // We have the full message 
    //if (NewTs232Frame == MSG_COMPLETE)
    //    processCmdIN();
    
    // Something goes wrong
    if (NewTs232Frame == MSG_ERROR)
    {
        if (LenRs232 > 0)
            printci_tx('e', Err_Code);
        NewTs232Frame = MSG_ANY;
        //flashAliveLED(5, CTR_LED_ERROR);  
    }
    
    

    return;
}

void interrupt ISR(void){

    if(PIE1bits.RCIE && PIR1bits.RCIF){ // handle RX pin interrupts
        UBYTE c;        /* Caractere recu */
        /* Lecture de l'octet recu */
        c = getch ();
        if (c!=0) // 0 means break characters used to wake up the PIC
            processRXIN(c);
        
        if(RCSTAbits.FERR){
            flagRXFramingError = 1;
            SPEN = 0;
            SPEN = 1;

        }
        if(RCSTAbits.OERR){
            flagRXOverrunError = 1;
            CREN = 0;
            CREN = 1;
        }
        
    } // end RX pin interrupt handlers
    
    // not used anomore
    //if (INTCONbits.RAIE && INTCONbits.RAIF){ // handle Channel pin interrupts
        /*ProcessReadChannel ();
        if (channel > 0 && channel <= 5)
            printci_tx('l', channel);
        else print_tx("l0", 2);*/
      /*   printci_tx('1',PIN_IN_LED_1);printci_tx('2',PIN_IN_LED_2);printci_tx('3',PIN_IN_LED_3);printci_tx('4',PIN_IN_LED_4);printc_tx('-');
            
        INTCONbits.RAIF = 0;
    }*/
    
    /*else if(INTCONbits.T0IF && INTCONbits.T0IE)  // handle Timer interrupts
    {                                           // if timer flag is set & interrupt enabled
        TMR0 -= 250;                            // reload the timer - 250uS * 256 = 64ms per interrupt
        INTCONbits.T0IF = 0;                    // clear the interrupt flag 
        if (scaleCount > 0 ) scaleCount --;     // we toggle LED only scale x 256 x 250 = 64 x scale (if scale = 4 => 256ms)
        else
        {
            scaleCount = scaler;
            PIN_OUT_ALIVE_LED = !PIN_OUT_ALIVE_LED; // toggle a bit to say we're alive
            if (PIN_OUT_ALIVE_LED == 1)
            {
                if (tCount > 0) tCount --;
                else INTCONbits.T0IE = 0;           // stop the timer 
            }
        }
    }*/
    
    
} // end ISRs*/


int main(int argc, char** argv) {
    unsigned char input;

    /*unsigned int p1 = 0;
    unsigned int p2 = 0;
    unsigned int p3 = 0;
    unsigned int p4 = 0;*/
    INTCON=0; //disable interrupts
    ANSEL = 0x00;
    TRISA = ALL_INPUTS; //PORTA_DIR; 
    TRISC = PORTC_BTN_RELEASED; 
    ADCON0 = 0x00;
    CMCON0 = 7;
    //OSCCON = 0x71; // switch to 8MHz system clock, internal oscillator
    
    // Temporaire : activation pull-up sur RA2
    //OPTION_REGbits.nRAPU = 1;
    //WPUAbits.WPU0 = 1;

    PIN_OUT_SW_CHANNEL = 1;
    PIN_OUT_SW_BOTTOM = 1;
    PIN_OUT_SW_TOP = 1;
    PIN_OUT_SW_MY = 1;
    PIN_OUT_SW_PROG = 1;
    /*ReleaseBtnUp();
    ReleaseBtnDown();
    ReleaseBtnMy();
    ReleaseBtnChannel();
    ReleaseBtnProg();*/
    //PIN_OUT_ALIVE_LED = 1;
    OPTION_REGbits.T0CS = 0;               // Timer increments on instruction clock
    INTCONbits.T0IE = 0;               // Disable interrupt on TMR0 overflow
    OPTION_REGbits.INTEDG = 0;             // falling edge trigger the interrupt
    //OPTION_REGbits.PSA = 0;            // prescaler assigned to Timer0
    //OPTION_REGbits.PS = 7;              // PS0, 1 and 2 to 1 => prescale to 256 (256us/cycle)
    INTCONbits.INTE = 0;               // disable the external interrupt
    INTCONbits.RAIE = 0;                // Disable Port A interrupts
    
    IOCA = 0x0F;        // Enable interrupts on Port A for RA0, RA1, RA2 and RA3
    PIE1bits.TXIE = 0; // Enable USART Transmitter interrupt
    PIE1bits.RCIE = 1; // Enable USART Receive interrupt
    INTCONbits.PEIE = 1; // Enable peripheral interrupts
    INTCONbits.GIE = 1; // Enable global interrupts
    
    // config for serial comms is set in init_comms
    init_comms();
    //delay_ms(5000);
    print_tx ("Start", 5);
    //flashAliveLED(10, CTR_LED_STARTUP); // LED de vie pour indiquer qu'on démarre
    
     while(1) {
        CLRWDT();       // Idly kick the dog
        // We go to bed only if the timer is off (not during blinking Led)
        // Test LED values
        /*if (p1 != !PIN_IN_LED_1)
        {p1=!PIN_IN_LED_1; printci_tx('1',p1);}
        if (p2 != !PIN_IN_LED_2)
        {p2=!PIN_IN_LED_2; printci_tx('2',p2);}
        if (p3 != !PIN_IN_LED_3)
        {p3=!PIN_IN_LED_3; printci_tx('3',p3);}
        if (p4 != !PIN_IN_LED_4)
        {p4=!PIN_IN_LED_4; printci_tx('4',p4);}*/

        // We go to bed only if the timer is off (not during blinking Led)
        if (INTCONbits.T0IE == 0 && BAUDCTLbits.RCIDL && NewTs232Frame == MSG_ANY)
        {
            fast_delay_ms();
            BAUDCTLbits.WUE = 1; // Enable Wake-up on reception on RX
            
            SLEEP();        // PIC is sleeping now
            //flashAliveLED(2, CTR_LED_RUNTIME); //We woke up
        }
        // We have the full message 
        if (NewTs232Frame == MSG_COMPLETE)
            processCmdIN();
    }

    return (0);
}
