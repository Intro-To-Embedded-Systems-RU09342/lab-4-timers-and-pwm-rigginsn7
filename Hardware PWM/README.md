# Hardware PWM
In this part of the lab I replicated the behavior of the software PWM but with hardware PWM. The LED duty cycle would be intially set to 50% and when the button is pressed it increases by 10 % every time until it reaches 100% then it resets to 0% on the next button press. 2 timers were instantiated 1 was used for the button deboucing and the other used the processors interal PWM function. 

## Processors Utilized
MSP430FR6989
MSP430G2553

## Dependencies
The only dependency is the library called MSP430.h. this is used for all TI MSP 430 processors.

## Differences in the Processors
For the MSP430FR6989 I had to disable the GPIO power on default high impedence mode. This allowed me to activate previously configured port settings. If you don't do this you can not use the on board LEDs. The only other difference in the code is the pin assignments specific to each board
