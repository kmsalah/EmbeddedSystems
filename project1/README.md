#Project 1
##Lighting up the DE2 Board

Added eight 7-bit parallel I/O modules to SOPC design corresponding to each of the DE2's seven segment displays.

Code for NIOS2 Processor changes the value of seven-segment displays depending on the period of the LEDs:

* 250ms: displays light with random configurations
* 125ms: displays follow a pattern where one segment is always lit and the lit segment follows a clockwise circular path around outermost ring of segments
* 50ms: displays show decimeal count from 0 to 99,999,999 and repeat

[Demo](https://youtu.be/dbLtYYW4uJ8)