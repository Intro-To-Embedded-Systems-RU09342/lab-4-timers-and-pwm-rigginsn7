/*Author Nicholas Riggins
 * Date Created : 10/13/19
 * Software PWM Lab 4
 * Processor : MSP430FR6989
 * Software PWM
 *
 */
// when the button is pressed P9.7 is toggled while P1.0 is getting brighter
// this is done using software PWM instead of hardware PWM

#include <msp430.h>


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       //stop watchdog timer (WDT)

    PM5CTL0 &= ~LOCKLPM5;           /* Disable the GPIO power-on default high-impedance mode
                                      to activate previously configured port settings*/

    P1DIR |= BIT0;                  // Set P1.0  to output direction
    P1SEL0 |= BIT0;                 // Set P1.0  to utilize PWM
    P9DIR |= BIT7;                  // Set P9.7 to output direction
    P9OUT &= ~BIT7;                 // Turn P9.7 off

    P1DIR &= ~BIT1;                 // Set P1.1 to input
    P1REN |= BIT1;                  // Enables interal resistor so it can connect to VCC and ground
    P1OUT |= BIT1;                  // Set P1.1 as a pull up resistor
    P1IES |= BIT1;                  // Interrupt enable on low to high edge for P1.1
    P1IE |= BIT1;                   // Interrupt enable for P1.1
    P1IFG &= ~BIT1;                 // Clear interrupt flags for P1.1

    TA1CTL = TASSEL__SMCLK | MC__CONTINUOUS; // Timer A0 is initialized
    TA1CCR0 = 64;                            // Debounce time is set
    TA1CCTL0 |= CCIE;                        // Enable interrupts

    TA0CTL = TASSEL__SMCLK | MC__UP;         // Timer A1 is initialized
    TA0CCR0 = 1000;                          // 100% duty cycle
    TA0CCR1 = 500;                           // 50% duty cycle
    TA0CCTL1 |= OUTMOD_7;                    // Resets output at CCR0 and sets output at CCR1

    __bis_SR_register(LPM0_bits + GIE);      // Interrupt is enabled in Low-power mode 0


   while(1){}
}
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
{
    P1IFG &= ~BIT1;                             // Interrupt flag is cleared
    TA1R = 0;                                   // Timer is reset
    TA1CTL = TASSEL__SMCLK | MC__CONTINUOUS;    // Start in continuous mode
}

void __attribute__((interrupt(TIMER1_A0_VECTOR))) timer1_A0 (void)
{
    TA1CTL &= ~TAIFG;                           // Interrupt flag is cleared
    TA1CTL &= ~MC_3;                            // Deboucing stops

    if(!(P1IN & BIT1) && (P1IES & BIT1))       // If the P1.1 is pressed
    {
        P1IES &= ~BIT1;                        // Change P1.1 interrupt edge
        P9OUT |= BIT7;                         // Turn on P9.7

        if(TA0CCR1 <= 900)                     // If the duty cycle is less than or equal to 90%
        {
            TA0CCR1 += 100;                    // The duty cycle is increased by 10%
            TA0CTL |= MC_1;                    // Up mode
            TA0CCTL1 |= OUTMOD_7;              // Output mode is changed
        }
        else
        {
            TA0CCR1 = 0;                       // Set duty cycle back to 0%
            TA0CTL &= ~MC_3;                   // Timer is stopped
            TA0CCTL1 &= ~OUTMOD_7;             // Turn output off
        }
    }

    if((P1IN & BIT1) && !(P1IES & BIT1))       // If P1.1 is released
    {
        P1IES |= BIT1;                        // Button interrupt edge is changed
        P9OUT &= ~BIT7;                       // Turn off P9.7  off
    }
}
