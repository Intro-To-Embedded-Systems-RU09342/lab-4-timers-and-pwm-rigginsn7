# Software Debouncing
In this part of the lab I had to utilize a debouncing signal. This has to be done was adding a mechanical switch to digital circuits so when the button is pressed once it doesn't appear like multiple presses. This insures that you don't get false positives. In this part for the MSP430FR6989 the button would turn the LED on and off and for the MSP430G2553 when the button is pressed down the LED will toggle on and off and when its let go it wi stay at the state it was last at. Since theres a debouncing signal it will prevent the board fromo reading multiple pressed from the switch.

## Processors Utilized
MSP430G2553
MSP430FR6989

## Dependencies
The only dependency is the library called MSP430.h. this is used for all TI MSP 430 processors.

## Differences in the Processors
For the MSP430FR6989 I had to disable the GPIO power on default high impedence mode. This allowed me to activate previously configured port settings. If you don't do this you can not use the on board LEDs. The only other difference in the code is the pin assignments specific to each board
