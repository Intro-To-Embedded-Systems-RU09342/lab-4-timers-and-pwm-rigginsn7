# Software Debouncing
In this part of the lab I had to utilize a debouncing signal. This has to be done was adding a mechanical switch to digital circuits so when the button is pressed once it doesn't appear like multiple presses. 
Debouncing a signal is a necessary step when adding mechanical switches to digital circuits so a single press doesn't appear like multiple presses. You're going to need this feature in a later portion of this lab as you'll be using a button press to change the duty cycle of your signal and you don't want to count the "false positives" as a button press. Debouncing is also only one of many applications which would require the use of built in Timers to allow for other processes to take place. Check out this nice tutorial on the subject. [http://www.labbookpages.co.uk/electronics/debounce.html]

## Processors Utilized
MSP430G2553
MSP430FR6989

## Dependencies
The only dependency is the library called MSP430.h. this is used for all TI MSP 430 processors.

## Differences in the Processors
For the MSP430FR6989 I had to disable the GPIO power on default high impedence mode. This allowed me to activate previously configured port settings. If you don't do this you can not use the on board LEDs. The only other difference in the code is the pin assignments specific to each board
