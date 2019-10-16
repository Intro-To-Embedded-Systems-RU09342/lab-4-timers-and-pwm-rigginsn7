/*Author Nicholas Riggins
 * Date Created : 10/13/19
 * Software PWM Lab 4
 * Processor : MSP430G2553
 * Software PWM
 *
 */


// when the button is pressed P1.6 is toggled while P1.0 is getting brighter
// this is done using software PWM instead of hardware PWM
#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           //stop watchdog timer (WDT)

    P1DIR |= BIT0;                      // Set P1.0 to output direction
    P1DIR |= BIT6;                      // Set P1.6  to output direction
    P1OUT &= ~BIT6;                     // Turn P1.6 off

    P1DIR &= ~BIT3;                     // Set P1.3 to input direction
    P1REN |= BIT3;                      // Enables internal resistor so it can connect to ground and VCC
    P1OUT |= BIT3;                      // Set P1.3 as a pull up resistor
    P1IES |= BIT3;                      // Interrupt enable on low to high edge for P1.3
    P1IE |= BIT3;                       // Interrupt enable for P1.3
    P1IFG &= ~BIT3;                     // Clear interrupt flags for P1.3

    TA1CTL = TASSEL_2 | MC_2;           // Timer A0 is initialized
    TA1CCR0 = 64;                       // Debounce time is set
    TA1CCTL0 |= CCIE;                   // Interrupt is enabled
    TA1CTL |= TAIE;                     // Enable interrupts

    TA0CTL = TASSEL_2 | MC_1;           // Timer A1 is initialized
    TA0CCR0 = 1000;                     // 100% duty cycle
    TA0CCR1 = 500;                      // 50% duty cycle

    TA0CCTL0 |= CCIE;                   // Interrupt is enabled
    TA0CCTL1 |= CCIE;
    TA0CTL |= TAIE;

    __bis_SR_register(LPM0_bits + GIE); // Interrupt is enabled in Low-power mode 0

   while(1){}
}
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
{
    P1IFG &= ~BIT3;             // Interrupt flag is cleared
    TA1R = 0;                   // Timer is reset
    TA1CTL = TASSEL_1 | MC_2;   // Start in continuous mode
}

void __attribute__((interrupt(TIMER1_A0_VECTOR))) timer1_A0 (void)
{
    TA1CTL &= ~TAIFG;           // Interrupt flag is cleared
    TA1CTL &= ~MC_3;            // Debouncing is stopped

    if(!(P1IN & BIT3) && (P1IES & BIT3)) // If P1.3 is pressed
    {
        P1IES &= ~BIT3;                 // Change P1.3 interrupt edge
        P1OUT |= BIT6;                  // Turn on P1.6

        if(TA0CCR1 <= 900)              // If the duty cycle is less than or equal to 90%
        {
            TA0CCR1 += 100;             // The duty cycle is increased by 10%
            TA0CTL |= MC_1;             // Up mode
        }
        else
        {
            TA0CCR1 = 0;                // Duty cycle is set back to 0
            TA0CTL &= ~MC_3;            // Timer is stopped
            P1OUT &= ~BIT0;             // P1.0 is turned off
        }
    }

    if((P1IN & BIT3) && !(P1IES & BIT3)) // If the P1.3 is released
    {
        P1IES |= BIT3;              // P1.3 interrupt edge is changed
        P1OUT &= ~BIT6;             // Turn off P1.6
    }
}

void __attribute__((interrupt(TIMER0_A0_VECTOR))) timer0_A0 (void)
{
    TA0CTL &= ~TAIFG;               // Interrupt flag is cleared
    P1OUT |= BIT0;                  // Turn on P1.0
}

void __attribute__((interrupt(TIMER0_A1_VECTOR))) timer0_A1 (void)
{
    if(TA0IV == 0x02)               // If the interrupt was generated from CCR1 compare
    {
        P1OUT &= ~BIT0;             // Turn off P1.0
    }
}
