/*Author Nicholas Riggins
 * Date Created : 10/12/19
 * Hardware PWM Lab 4
 * Processor : MSP430FR6989
 * Hardware PWM
 *
 */


// when the button is pressed led P9.7 toggles and led P1.0 gets brighter
#include <msp430.h>

volatile int duty = 500; // Value of the duty cycle, which is at 50% to start off

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;        // Stop watchdog timer (WDT)


    PM5CTL0 &= ~LOCKLPM5;   // Disable the GPIO power-on default high-impedance mode to activate
                            // previously configured port settings

    P9DIR |= BIT7;  // Set P9.7 to output direction
    P1DIR |= BIT0;  // Set P1.0  to output direction
    P1SEL0 |= BIT0;  // Set P1.0  to PWM
    P1DIR &= ~BIT1; // Set P1.1  to input direction
    P1OUT =  BIT1;  // Set P1.1 as a pull up resistor
    P1REN |= BIT1;  // Enables internal resistor so it can connect to ground and VCC
    P1IE |= BIT1;   // Interrupt enable for P1.1
    P1IES |= BIT1;  // Interrupt enable on low to high edge for P1.1
    P1IFG &= ~BIT1; // Clear interrupt flags for P1.1

    TA0CCR0 = 1000;       // 100% duty cycle
    TA0CCTL1 = OUTMOD_7;  // Set output mode
    TA0CCR1 = duty;       // Set to duty cycle
    TA0CTL = TASSEL_2 + MC_1; // Timer is initialized

     __bis_SR_register(LPM0_bits + GIE);  // Interrupt is enabled in LPM0 (Low-power mode 0)
    }

#pragma vector=PORT1_VECTOR        // Interrupt triggers on button press
__interrupt void Button(void)
{
          P9OUT ^= BIT7;    // Debugging P9.7 toggle
     if (duty == 1000)
     {        // If duty cycle is set at 100%
            duty = 0;          // Set duty cycle back to 0%
     }
     else
     {
            duty += 100;   // Duty cycle increments by 10%
     }
    TA0CCR1 = duty;          // Set duty cycle
    P1IFG = ~BIT1;            // Interrupt flag off
}
