

#include <stdio.h>
#include <unistd.h>
#include "system.h"
#include "stdlib.h"
#include "altera_avalon_pio_regs.h"
#include "alt_types.h"

#include <altera_up_avalon_video_character_buffer_with_dma.h>  // to write characters to video
#include <altera_up_avalon_video_pixel_buffer_dma.h> // to swap front and back buffer
#include <math.h>  // for trigonometry functions


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

const int rows = 240;
const int cols = 320;
const int radians = M_PI/180;
alt_up_pixel_buffer_dma_dev *my_pixel_buffer;



float rgbVals[3] = {0};
int scaleState = 10;

void getInterpoledRGBVals(float row, float col, alt_u8 * image);

void printPixel(float red, float green, float blue, int col, int row);

int main()
{
  int degrees = 10;
  printf("Hello from Nios II!\n");
  my_pixel_buffer= alt_up_pixel_buffer_dma_open_dev("/dev/video_pixel_buffer_dma_0");

  FILE *myfile;
  myfile = fopen("my_fs/myfile.dat","rb");
  if (myfile == NULL) perror ("error opening datafile");

  alt_u8 *my_image;
  my_image=(alt_u8 *)malloc(320*240*3);
  fread(my_image,sizeof(alt_u8),320*240*3,myfile);



  alt_up_pixel_buffer_dma_clear_screen (my_pixel_buffer,0 );
  alt_u32 current_value;
  alt_u32 current_state;
  alt_u8 current_direction;
  alt_u32 keys;

  current_state=3;
  current_value=1;
  current_direction=0;

  printf ("Program running (UART)...\n");
  fprintf (stderr,"Program running (LCD)...\n");
  while (1)
  {
	  alt_up_char_buffer_dev * my_char_buffer;
	  my_char_buffer =  alt_up_char_buffer_open_dev("/dev/video_character_buffer_with_dma_0");
	  if (! my_char_buffer)  printf("error opening character buffer  \ n");


  // read the current state of the keys
	  keys=IORD_ALTERA_AVALON_PIO_DATA(KEYS_BASE);
	  // switch speed if necessary
	  if ((keys != 7) && (keys != current_state))
	  {
		  if (keys == 3)
		  {
			  alt_up_pixel_buffer_dma_clear_screen (my_pixel_buffer,0 );
			  printf ("KEY3 \n");
		  }
		  else if (keys == 5)
		  {
			  degrees = 10;
			  alt_up_pixel_buffer_dma_clear_screen (my_pixel_buffer,0 );
			  printf ("KEY2 \n");
		  }
		  else if (keys == 6)
		  {
			  alt_up_pixel_buffer_dma_clear_screen (my_pixel_buffer,0 );
			  printf ("KEY1 \n");

		  }
		  current_state=keys;
	  }


	  if(current_state==3) //KEY3 (ORIGINAL)
	  {
		  alt_up_char_buffer_clear(my_char_buffer );
		  alt_up_char_buffer_string( my_char_buffer,"Original Image",0,59);

		  /*
		  IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE,rand() % 128); //for each display, use a random value from 0-127
		  IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE,rand() % 128);
		  IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE,rand() % 128);
		  IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE,rand() % 128);
		  IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE,rand() % 128);
		  IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE,rand() % 128);
		  IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE,rand() % 128);
		  IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE,rand() % 128);
		  */

		  int k;
		  int l;


		  for(k = 0; k <= rows; ++k)

		  {

			for(l = 0; l <= cols; ++l)

			{

			  printPixel((my_image[( k*320*3+l*3+0)]), (my_image[( k*320*3+l*3+1)]), (my_image[( k*320*3+l*3+2)]),l, k);

			}
		  }

	  }
	  else if (current_state == 5) //KEY2 (ROTATION)
	  {

			  alt_up_char_buffer_clear(my_char_buffer );
			  alt_up_char_buffer_string( my_char_buffer,"Rotated Image",0,59);

			  float cosVal = cos(-degrees*M_PI/180);
			  float sinVal = sin(-degrees*M_PI/180);

			  int k;
			  int l;
			  for(k = 0; k < rows; ++k) //row - k
			  {
				int row = k-120;
			  	float rowCos = row*cosVal;
			  	float rowSin = row*sinVal;
				for(l = 0; l < cols; ++l) //col - l
				{
				  int col = l-160;
				  float destRow = (rowCos + col*sinVal);
				  float destCol = (-rowSin + col*cosVal);
				  destRow += 120;
				  destCol += 160;
				  if (destCol>= 0 && destCol <= cols && destRow <= rows && destRow >= 0)
				  {
					  getInterpoledRGBVals(destRow, destCol, my_image); //puts RGB vals in rgbVals[]
					  printPixel(rgbVals[0], rgbVals[1], rgbVals[2], l, k);
				  }
				}
			  }
			  degrees = degrees + 10;

			  if (degrees == 360)
			  {
				  degrees = 0;
			  }
			  alt_up_pixel_buffer_dma_clear_screen (my_pixel_buffer,0 );
	  }
	  else if (current_state ==6) //KEY1 (SCALING)
	  {
		alt_up_char_buffer_clear(my_char_buffer );
		alt_up_char_buffer_string( my_char_buffer,"Scaling Image",0,59);

		int k;
	    int l;
	    float scale = (abs(scaleState)/10.0);
        printf("%f \n", scale);
	    for(k = 0; k < rows; k++)
	    {
	      int row = k-120;
		  //alt_u16 scaledRow = (alt_u16)roundf(row * scale);
	      float scaledRow = row / scale;
          scaledRow += 120;

		  for(l = 0; l < cols; l++)
		  {
			int col = l-160;
			//alt_u16 scaledCol = (alt_u16)roundf(col * scale);
			float scaledCol = col / scale;
			scaledCol += 160;

			//printPixel((my_image[( k*320*3+l*3+2)]), (my_image[( k*320*3+l*3+1)]), (my_image[( k*320*3+l*3+0)]),scaledCol, scaledRow);
			if (scaledCol>= 0 && scaledCol <= cols && scaledRow <= rows && scaledRow >= 0)
			{  getInterpoledRGBVals(scaledRow, scaledCol, my_image);
			   printPixel(rgbVals[0], rgbVals[1], rgbVals[2], l, k);
			   //printPixel((my_image[( k*320*3+l*3+0)]), (my_image[( k*320*3+l*3+1)]), (my_image[( k*320*3+l*3+2)]),scaledCol, scaledRow);

			}
		  }
	    }
	      scaleState--;

	    if(scaleState == 0)
	    {
		  scaleState = -2;
	    }
	    else if(scaleState <= -10)
	    {
		  scaleState = 10;
	    }

	  alt_up_pixel_buffer_dma_clear_screen (my_pixel_buffer,0 );
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

void getInterpoledRGBVals(float row, float col, alt_u8 * my_image)
{

	float rowFrac = row - (int)row; //iFrac
	float colFrac =  col - (int)col; //jFrac

	int tlRow = row - rowFrac; //i
	int tlCol = col - colFrac; //j



	float weightTopLeft = (1-rowFrac)*(1-colFrac); //weight(i, j)
	float weightTopRight = (1-rowFrac)*colFrac; //weight(i,j+1)
	float weightBotLeft = rowFrac*(1-colFrac); //weight(i+1, j)
	float weightBotRight = rowFrac*colFrac; //weight(i+1, j+1)

	int red =
			weightTopLeft*(my_image[(tlRow*960+tlCol*3)])
			+ weightTopRight*(my_image[(tlRow*960+(tlCol+1)*3)])
			+ weightBotLeft*(my_image[((tlRow+1)*960+(tlCol)*3)])
			+ weightBotRight*(my_image[((tlRow+1)*960+(tlCol+1)*3)]);

	float green =
			weightTopLeft*(my_image[(tlRow*960+tlCol*3+1)])
			+ weightTopRight*(my_image[(tlRow*960+(tlCol+1)*3+1)])
			+ weightBotLeft*(my_image[((tlRow+1)*960+(tlCol)*3+1)])
			+ weightBotRight*(my_image[((tlRow+1)*960+(tlCol+1)*3+1)]);
	float blue =
			weightTopLeft*(my_image[(tlRow*960+tlCol*3+2)])
	  		 + weightTopRight*(my_image[(tlRow*960+(tlCol+1)*3+2)])
	  		 + weightBotLeft*(my_image[((tlRow+1)*960+(tlCol)*3+2)])
	  		 + weightBotRight*(my_image[((tlRow+1)*960+(tlCol+1)*3+2)]);

	rgbVals[0] = red;
	rgbVals[1] = green;
    rgbVals[2] = blue;
}



void printPixel(float red, float green, float blue, int col, int row)
{
	alt_up_pixel_buffer_dma_draw(my_pixel_buffer,(blue +
							  ((int)green<<8) +((int)red<<16)),col, row);
}









