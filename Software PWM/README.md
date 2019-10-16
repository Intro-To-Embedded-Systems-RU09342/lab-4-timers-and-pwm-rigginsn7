# Software PWM
For this part of the lab I have an LED start up at 50% of the duty cycle at the start. Then when the on board button is pressed it will increase the LED duty cycle 10% until it reaches 100% then it resets to 0% then it restarts from 0 and continues to increment the duty cycle by 10%. There are 2 timers being used. The first timer is used for the debouncing the button. The second is used for sending the PWM signal to the LED.

## Processors Utilized 
MSP430FR6989
MSP430G2553

## Dependencies
The only dependency is the library called MSP430.h. this is used for all TI MSP 430 processors.

## difference in the Processors
For the MSP430FR6989 I had to disable the GPIO power on default high impedence mode. This allowed me to activate previously configured port settings. If you don't do this you can not use the on board LEDs. The only other difference in the code is the pin assignments specific to each board
