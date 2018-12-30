/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include "system.h"
#include "stdlib.h"
#include "altera_avalon_pio_regs.h"
#include "alt_types.h"

const int _OFF = 127; //NO segments on LED are lit
//Constant definitions for specific segments of the LED
const int _TOP = 254;
const int _BOTTOM = 247;
const int _TOPRIGHT = 253;
const int _BOTTOMRIGHT = 251;
const int _BOTTOMLEFT = 239;
const int _TOPLEFT= 223;
//Constant definitions full digits on the 7 segment display
const int _ZERO = 64;
const int _ONE = 121;
const int _TWO = 36;
const int _THREE = 48;
const int _FOUR = 25;
const int _FIVE = 18;
const int _SIX = 2;
const int _SEVEN = 120;
const int _EIGHT = 128;
const int _NINE = 16;


int numToConst(int digit);

int main()
{
  printf("Hello from Nios II!\n");
  alt_u32 current_value;
  alt_u32 current_state;
  alt_u8 current_direction;
  alt_u32 keys;

  current_state=3;
  current_value=1;
  current_direction=0;

  printf ("Program running (UART)...\n");
  fprintf (stderr,"Program running (LCD)...\n");
  int i = 0; //counter for the number counter
  int j = 0; //counter for the lighted path
  while (1)
  {
  // read the current state of the keys
	  keys=IORD_ALTERA_AVALON_PIO_DATA(KEYS_BASE);
	  // switch speed if necessary
	  if ((keys != 7) && (keys != current_state))
	  {
		  if (keys == 3)
		  {
			  printf ("speed set to 250 ms\n");
		  	  //random
		  }
		  else if (keys == 5)
		  {
			  printf ("speed set to 150 ms\n");
			  j = 0;
		  	  //light going around edge
		  }
		  else if (keys == 6)
		  {
			  printf ("speed set to 50 ms\n");
			  i = 0;
			  //Counting up
		  }
		  current_state=keys;
	  }


	  if(current_state==3) //if speed is 250ms, display random configurations
	  {
		  IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE,rand() % 128); //for each display, use a random value from 0-127
		  IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE,rand() % 128);
		  IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE,rand() % 128);
		  IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE,rand() % 128);
		  IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE,rand() % 128);
		  IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE,rand() % 128);
		  IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE,rand() % 128);
		  IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE,rand() % 128);
	  }
	  else if (current_state == 5) //else if speed is 150, light follows path around display
	  {
		  /*
		   * There are eight 7-segment displays, so for a light to follow a path around all 8,
		   * it would go across 20 positions. Here all 20 different positions are hardcoded. Int j
		   * serves as the counter. Through each iteration of the outer while loop, if the speed is
		   * set to 150, it will come to this switch statement and go to the right position based on j.
		   */

		  switch (j)
		  {
		  case 0:
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _TOP);
			  j++;
			  break;
		  case 1:
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _TOP);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _OFF);
			  j++;
			  break;
		  case 2:
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _TOP);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
			  IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _OFF);
			  j++;
			  break;
		  case 3:
              IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _OFF);
              IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
              IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
              IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
              IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _TOP);
              IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
              IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
              IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _OFF);
              j++;
              break;
		  case 4:
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _TOP);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _OFF);
		      j++;
		      break;
		  case 5:
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _TOP);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _OFF);
		      j++;
		      break;
		  case 6:
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _TOP);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _OFF);
		      j++;
		      break;
		  case 7:
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _TOP);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _OFF);
		      j++;
		      break;
		  case 8:
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _TOPRIGHT);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _OFF);
		      j++;
		      break;
		  case 9:
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _BOTTOMRIGHT);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _OFF);
		      j++;
		      break;
		  case 10:
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _BOTTOM);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _OFF);
		      j++;
		      break;
		  case 11:
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _BOTTOM);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _OFF);
		      j++;
		      break;
		  case 12:
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _BOTTOM);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _OFF);
		      j++;
		      break;
		  case 13:
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _BOTTOM);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _OFF);
		      j++;
		      break;
		  case 14:
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _BOTTOM);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _OFF);
		      j++;
		      break;
		  case 15:
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _BOTTOM);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _OFF);
		      j++;
		      break;
		  case 16:
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _BOTTOM);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _OFF);
		      j++;
		      break;
		  case 17:
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _BOTTOM);
		      j++;
		      break;
		  case 18:
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _BOTTOMLEFT);
		      j++;
		      break;
		  case 19:
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
		      IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _TOPLEFT);
		      j = 0;
		      break;
		  }


	  }
	  else if (current_state ==6) //if speed is 50ms
	  {
		  		IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE,numToConst(i%10)); //if i less than 10, just return remainder divided by 10 for first digit

		  		if (i >= 10) //if the digit is larger than 10
		  		{
		  			IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE,numToConst((i/10)%10)); //diving by 10 and getting modulo 10, will give second digit
		  		}
		  		else //if the digit is smaller than 10, do not light the display
		  		{
		  			IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, _OFF);
		  		}

		  		if (i >= 100) //same pattern as above for the rest
		  		{
		  			IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE,numToConst((i/100)%10));
		  		}
		  		else
		  		{
		  			IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, _OFF);
		  		}

		  		if (i >= 1000)
		  		{
		  			IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE,numToConst((i/1000)%10));
		  		}
		  		else
		  		{
		  			IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, _OFF);
		  		}

		  		if (i >= 10000)
		  		{
		  			IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE,numToConst((i/10000)%10));
		  		}
		  		else
		  		{
		  			IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, _OFF);
		  		}

		  		if (i >= 100000)
		  		{
		  			IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE,numToConst((i/10000)%10));
		  		}
		  		else
		  		{
		  			IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, _OFF);
		  		}

		  		if (i >= 1000000)
		  		{
		  			IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE,numToConst((i/1000000)%10));
		  		}
		  		else
		  		{
		  			IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, _OFF);
		  		}

		  		if (i >= 10000000)
		  		{
		  		    IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE,numToConst((i/10000000)%10));
		  		}
		  		else
		  		{
		  			IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, _OFF);
		  		}
		  		i++; //increment counter

		  		if (i == 99999999) // reset to zero when the number is 99999999
		  		{
		  			i = 0;
		  		}
	  }

	  // switch direction if necessary
	  if ((current_direction==0) && (current_value==(1 << 25)))
		  current_direction=1;
	  else if ((current_direction==1) && (current_value==1))
		  current_direction=0;
	  // move light
	  else if (current_direction==0)
		  current_value = current_value << 1;
	  else
		  current_value = current_value >> 1;

	  // update lights

	  IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE,current_value);

	  // wait
	  if (current_state==3)
		  usleep (250000);
	  else if (current_state==5)
		  usleep (125000);
	  else usleep (50000);
  }
  return 0;
}

/*
 * Grabs the constant definitons of a full digit for you.
 */
int numToConst(int digit)
{
	switch(digit)
	{
	case 0:
		return _ZERO;
	case 1:
		return _ONE;
	case 2:
		return _TWO;
	case 3:
		return _THREE;
	case 4:
		return _FOUR;
	case 5:
		return _FIVE;
	case 6:
		return _SIX;
	case 7:
		return _SEVEN;
	case 8:
		return _EIGHT;
	case 9:
		return _NINE;
	default:
		return _ZERO;
	}
}












