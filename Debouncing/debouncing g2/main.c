/*Author Nicholas Riggins
 * Date Created : 10/9/19
 * Debouncing Lab 4
 * Processor : MSP430G2553
 * Implement a debouncing signal so when a mechanical switch is used on
 * a digital circuit so you dont get false positives as a button press
 *
 */


// if you hold the button the LED will start to blink
#include <msp430.h>

int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer (WDT)

    P1DIR |= BIT6;                  // Set P1.6 to the output direction
    P1OUT &= ~BIT6;                 // Turn off P1.6

    P1DIR &= ~BIT3;                 // Set P1.3 to input direction
    P1REN |= BIT3;                  //Enables internal resistor so it can connect to 3.6 v and ground
    P1IE |= BIT3;                   // Interrupt is enabled on P1.3
    P1IES |= ~BIT3;                 // Set interrupt flag on high to low for P1.3
    P1IFG &= ~BIT3;                 // Clear interrupt flag for P1.3


    TA0CCR0 = 32;                   // Set value for debouncing time
    TA0CCTL0 |= CCIE;               // Interrupt is enabled
    TA0CTL |= TASSEL_1 + MC_2;      // Timer 0 is set up

    __enable_interrupt();           // Interrupt is enabled

    __bis_SR_register(LPM0 + GIE);  // Interrupt is enabled in LPM0 (Low-power mode 0)

    while(1)
    {
            }
}

void __attribute__((interrupt(TIMER0_A0_VECTOR))) Timer_A0 (void)
{
    TA0CTL &= ~TAIFG;       // Interrupt flag is cleared
    TA0CTL |= MC_0;         // Timer is stopped

    if(!(P1IN & BIT3))      // If P1.3 is pressed led will toggle
        P1OUT ^= BIT6;
}

void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
{
        P1IFG &= ~BIT3;     // Interrupt flag is cleared
        TA0R = 0;           // Timer is reset
        TA0CTL |= MC_2;     // Start continuous mode
}
