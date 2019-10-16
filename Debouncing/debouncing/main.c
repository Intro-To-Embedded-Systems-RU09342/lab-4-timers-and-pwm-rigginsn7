/*Author Nicholas Riggins
 * Date Created : 10/9/19
 * Debouncing Lab 4
 * Processor : MSP430FR6989
 * Implement a debouncing signal so when a mechanical switch is used on
 * a digital circuit so you dont get false positives as a button press
 *
 */



// the button will turnt the led off and on
#include <msp430.h>

int main(void)
{
        WDTCTL = WDTPW | WDTHOLD;        //stop watchdog timer (WDT)

        PM5CTL0 &= ~LOCKLPM5;            /* Disable the GPIO power-on default high-impedance mode
                                         to activate previously configured port settings*/

        P1DIR |= BIT0;                   // Set P1.0 to output direction
        P1OUT &= ~BIT0;                  // Turn off P1.0
        P1DIR &= ~BIT1;                  // Set P1.3 to input direction
        P1REN |= BIT1;                   // Enables internal resistor so it can connect to 3.6v and ground
        P1OUT |= BIT1;                  // Interrupt is enabled on P1.1
        P1IFG = 0x00;                   // Clear interrupt flag
        P1IES |= BIT1;                  // P1.1 press triggers interrupts
        P1IE |= BIT1;                   // Interrupt is enabled on P1.1

        TA0CTL |= TASSEL_2;             // SMCLK
        TA0CTL |= MC_2;                 // Continuous mode
        TA0CCR0 = 0x0041;               // Timer count up to 2ms
        TA0CCTL0 |= CCIE;               // Interrupt is enabled
        TA0CTL |= TAIE;                 // Timer 0 is set up

        __bis_SR_register(LPM0_bits + GIE);     // Interrupt is enabled in LPM0 (Low-power mode 0)


        while(1) {   // loop will always run
                  __no_operation();
              }

}


void __attribute__((interrupt(TIMER0_A0_VECTOR))) Timer_A0 (void)
{
        TA0CTL &= ~TAIFG;       // Interrupt flag is cleared
        TA0CTL &= ~MC_3;        // Stops timer

        if(!(P1IN & BIT1))
        {
            P1OUT ^= BIT0;       // The P1.0 will toggle on and off
        }


}

// Button Interrupt
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
{
        P1IFG &= ~BIT1;         // Interrupt flag is cleared

        TA0R = 0;               // Timer is reset
        TA0CTL &= ~MC_3;
        TA0CTL |= MC_2;         // Timer is reset
}
