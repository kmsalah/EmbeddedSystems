
#include <stdio.h>
#include <unistd.h>
#include "system.h"
#include "stdlib.h"
#include "altera_avalon_pio_regs.h"
#include "alt_types.h"
#include <altera_avalon_performance_counter.h>
#include <altera_up_avalon_video_character_buffer_with_dma.h>  // to write characters to video
#include <altera_up_avalon_video_pixel_buffer_dma.h> // to swap front and back buffer
#include <math.h>  // for trigonometry functions
const int rows = 240;
const int cols = 320;
const int radians = M_PI / 180;
alt_up_pixel_buffer_dma_dev *my_pixel_buffer;

int fixedRBGVals[3] = { 0 };
int time = 0;
float totalRotationTime = 0;


void getFixedInterpoledRBGVals(alt_32 row, alt_32 col, alt_u8 * image);

void printFPixel(int red, int green, int blue, int col, int row);
void printPixel(float red, float green, float blue, int col, int row);

int main() {
	//altera_avalon_mailbox_open();
	//altera_avalon_mailbox_open();
	int degrees = 10;
	printf("Hello from Nios II!\n");
	my_pixel_buffer = alt_up_pixel_buffer_dma_open_dev(
			"/dev/video_pixel_buffer_dma_0");

	FILE *myfile;
	myfile = fopen("my_fs/myfile.dat", "rb");
	if (myfile == NULL)
		perror("error opening datafile");

	alt_u8 *my_image;
	my_image = (alt_u8 *) malloc(320 * 240 * 3);
	fread(my_image, sizeof(alt_u8), 320 * 240 * 3, myfile);

	alt_up_pixel_buffer_dma_clear_screen(my_pixel_buffer, 0);
	alt_u32 current_value;
	alt_u32 current_state;
	alt_u8 current_direction;
	alt_u32 keys;

	current_state = 3;
	current_value = 1;
	current_direction = 0;

	printf("Program running (UART)...\n");
	fprintf(stderr, "Program running (LCD)...\n");
	while (1) {
		alt_up_char_buffer_dev * my_char_buffer;
		my_char_buffer = alt_up_char_buffer_open_dev(
				"/dev/video_character_buffer_with_dma_0");
		if (!my_char_buffer)
			printf("error opening character buffer  \n");

		// read the current state of the keys
		keys = IORD_ALTERA_AVALON_PIO_DATA(KEYS_BASE);
		// switch speed if necessary
		if ((keys != 7) && (keys != current_state)) {
			if (keys == 3) {
				alt_up_pixel_buffer_dma_clear_screen(my_pixel_buffer, 0);
				printf("KEY3 \n");
			}
			current_state = keys;
		}

		if (current_state == 3)
		{
			alt_up_char_buffer_clear(my_char_buffer);
			alt_up_char_buffer_string(my_char_buffer, "Rotated Image", 0, 59);

			alt_32 cosVal = (alt_32) (cosf(-degrees * M_PI / 180) * 512); //these are fixed point
			alt_32 sinVal = (alt_32) (sinf(-degrees * M_PI / 180) * 512);

			int k;
			int l;

			PERF_RESET(PERFORMANCE_COUNTER_0_BASE);
			PERF_START_MEASURING(PERFORMANCE_COUNTER_0_BASE);

			int cpuid=__builtin_rdctl(5);

			for (k = cpuid; k < rows; k+=2) //row - k
			{
				printf("%i \n row", k);
				int row = (k - 120);
				alt_32 rowCos = (row * cosVal);
				alt_32 rowSin = (row * sinVal);

				for (l = 0; l < cols; ++l) {
					int col = (l - 160);

					alt_32 destRow = (rowCos + col * sinVal);
					alt_32 destCol = (-rowSin + col * cosVal);


					destRow += (120*512);
					destCol += (160*512);

					if (destCol >= 0 && destCol <= (cols*512) && destRow <= (rows*512)&& destRow >= 0)
					{
						getFixedInterpoledRBGVals(destRow, destCol, my_image);
						printFPixel(fixedRBGVals[0], fixedRBGVals[1],fixedRBGVals[2], l, k);
					}
				}
			}
			time = perf_get_total_time((void*) PERFORMANCE_COUNTER_0_BASE);
			time = time/(320*240); //average cycles per pixel
			totalRotationTime += time;
			printf("%i \n", time);
			PERF_START_MEASURING(PERFORMANCE_COUNTER_0_BASE); // restart counter

			degrees = degrees + 10;

			if (degrees == 360) {
				totalRotationTime = totalRotationTime / 36.0;
				printf("Average Number of Cycles per Pixel: %f \n", totalRotationTime);
				totalRotationTime = 0;
				degrees = 0;
			}
			alt_up_pixel_buffer_dma_clear_screen(my_pixel_buffer, 0);
		}
	}
}



void getFixedInterpoledRBGVals(alt_32 row, alt_32 col, alt_u8 * my_image) {

	int tlRow = row/512*512;
	int tlCol = col/512*512;

	alt_32 rowFrac = (row - tlRow);
	alt_32 colFrac = (col - tlCol);


	alt_32 weightTopLeft = (alt_32)((512-(rowFrac))*(512-(colFrac))); //weight(i, j)
	alt_32 weightTopRight = (alt_32)(((512-(rowFrac))*(colFrac))); //weight(i,j+1)
	alt_32 weightBotLeft = (alt_32)((rowFrac)*(512-(colFrac))); //weight(i+1, j)
	alt_32 weightBotRight = (alt_32)(((rowFrac)*(colFrac))); //weight(i+1, j+1)

	weightTopLeft /= 512;
	weightTopRight /= 512;
	weightBotLeft /= 512;
	weightBotRight /= 512;

	tlRow /= 512;
	tlCol /= 512;

	alt_32 red = weightTopLeft * (my_image[(tlRow * 960 + tlCol * 3)])
			+ weightTopRight * (my_image[(tlRow * 960 + (tlCol + 1) * 3)])
			+ weightBotLeft * (my_image[((tlRow + 1) * 960 + (tlCol) * 3)])
			+ weightBotRight
					* (my_image[((tlRow + 1) * 960 + (tlCol + 1) * 3)]);

	alt_32 green = weightTopLeft * (my_image[(tlRow * 960 + tlCol * 3 + 1)])
			+ weightTopRight * (my_image[(tlRow * 960 + (tlCol + 1) * 3 + 1)])
			+ weightBotLeft * (my_image[((tlRow + 1) * 960 + (tlCol) * 3 + 1)])
			+ weightBotRight * (my_image[((tlRow + 1) * 960 + (tlCol + 1) * 3
					+ 1)]);
	alt_32 blue = weightTopLeft * (my_image[(tlRow * 960 + tlCol * 3 + 2)])
			+ weightTopRight * (my_image[(tlRow * 960 + (tlCol + 1) * 3 + 2)])
			+ weightBotLeft * (my_image[((tlRow + 1) * 960 + (tlCol) * 3 + 2)])
			+ weightBotRight * (my_image[((tlRow + 1) * 960 + (tlCol + 1) * 3
					+ 2)]);

	fixedRBGVals[0] = (red / 512);
	fixedRBGVals[1] = (green / 512);
	fixedRBGVals[2] = (blue / 512);
}

void printPixel(float red, float green, float blue, int col, int row) {
	alt_up_pixel_buffer_dma_draw(my_pixel_buffer, (blue + ((int) green << 8)
			+ ((int) red << 16)), col, row);
}

void printFPixel(int red, int green, int blue, int col, int row) {
	alt_up_pixel_buffer_dma_draw(my_pixel_buffer, (blue + (green << 8)
			+ (red << 16)), col, row);
}

