/*Author Nicholas Riggins
 * Date Created : 10/12/19
 * Hardware PWMLab 4
 * Processor : MSP430G2553
 *Hardware PWM
 *
 */
// when the button is pressed led P1.0 is toggled while led P1.6 gets brighter as the duty cycle increases
#include <msp430.h>

volatile int duty = 500; //duty cycle value

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;        //stop watchdog timer

    P1DIR |= BIT0;                  // Set P1.0 to output direction
    P1DIR |= BIT6;                  // Set P1.6 to output direction
    P1SEL |= BIT6;                  // Set P1.6 to PWM
    P1OUT =  BIT3;                  // Sets P1.3 as a pull up resistor
    P1REN |= BIT3;                  // Enables internal resistor so it can be connected to VCC and ground
    P1IE |= BIT3;                   // Interrupt enable for P1.3
    P1IES |= BIT3;                  // Interrupt enable on low to high edge for P1.3
    P1IFG &= ~BIT3;                 // Clear interrupt flags for P1.3

    TA0CCR0 = 1000;                 // 100% duty cycle
    TA0CCTL1 = OUTMOD_7;            // Set output mode
    TA0CCR1 = duty;                 // Set to duty cycle
    TA0CTL = TASSEL_2 + MC_1;       // Timer is initialized

     __bis_SR_register(LPM0_bits + GIE);  // Interrupt is enabled in LPM0 (Low-power mode 0)

}

#pragma vector=PORT1_VECTOR       // Interrupt triggers on button press
__interrupt void Button(void)
    {
              P1OUT ^= BIT0;    // Debugging P1.0 toggle

     if (duty == 1000)          // If duty cycle is set at 100%
     {
            duty = 0;          // Set duty cycle back to 0%
        }
     else
     {
            duty += 100;        // Duty cycle increments by 10%
     }
        TA0CCR1 = duty;          // Set duty cycle
        P1IFG = ~BIT3;           // Interrupt flag off
    }
