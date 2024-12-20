/*
 * lcd.c
 *
 *  Created on: Dec 16, 2024
 *      Author: godmaze
 */

#include <string.h>
#include <stdlib.h>

#include "lcd.h"
//#include "font5x8.h"
#include "min_font.h"
#include "spi.h"

// Low-level LCD driving functions --------------------------------------------------------------------------

// Reset the LCD hardware
void lcdReset(void)
{
	// Reset pin is active low (0 = reset, 1 = ready)
	reset_res();
	LL_mDelay(100);

	set_res();
	LL_mDelay(100);
}

void lcdWriteCommand(uint8_t address)
{
	reset_cd();
	reset_cs();
	readWriteSPI1(address);
	set_cs();
}

void lcdWriteParameter(uint8_t parameter)
{
	set_cd();
	reset_cs();
	readWriteSPI1(parameter);
	set_cs();
}

void lcdWriteData(uint8_t dataByte1, uint8_t dataByte2)
{
	set_cd();
	reset_cs();
	readWriteSPI1(dataByte1);
	readWriteSPI1(dataByte2);
	set_cs();
}

void lcdInitialise(uint8_t orientation)
{
	// Set up the IO ports for communication with the LCD

	initCD_Pin();
	initCS_Pin();
	initRES_Pin();

	LL_mDelay(50);

	// Hardware reset the LCD
	lcdReset();

    lcdWriteCommand(EXIT_SLEEP_MODE);
    LL_mDelay(100); //Delay(10000); // Wait for the screen to wake up

    lcdWriteCommand(SET_PIXEL_FORMAT);
    lcdWriteParameter(0x05); // 16 bits per pixel

    lcdWriteCommand(SET_GAMMA_CURVE);
    lcdWriteParameter(0x04); // Select gamma curve 3

    lcdWriteCommand(GAM_R_SEL);
    lcdWriteParameter(0x01); // Gamma adjustment enabled

    lcdWriteCommand(POSITIVE_GAMMA_CORRECT);
    lcdWriteParameter(0x3f); // 1st Parameter
    lcdWriteParameter(0x25); // 2nd Parameter
    lcdWriteParameter(0x1c); // 3rd Parameter
    lcdWriteParameter(0x1e); // 4th Parameter
    lcdWriteParameter(0x20); // 5th Parameter
    lcdWriteParameter(0x12); // 6th Parameter
    lcdWriteParameter(0x2a); // 7th Parameter
    lcdWriteParameter(0x90); // 8th Parameter
    lcdWriteParameter(0x24); // 9th Parameter
    lcdWriteParameter(0x11); // 10th Parameter
    lcdWriteParameter(0x00); // 11th Parameter
    lcdWriteParameter(0x00); // 12th Parameter
    lcdWriteParameter(0x00); // 13th Parameter
    lcdWriteParameter(0x00); // 14th Parameter
    lcdWriteParameter(0x00); // 15th Parameter

    lcdWriteCommand(NEGATIVE_GAMMA_CORRECT);
    lcdWriteParameter(0x20); // 1st Parameter
    lcdWriteParameter(0x20); // 2nd Parameter
    lcdWriteParameter(0x20); // 3rd Parameter
    lcdWriteParameter(0x20); // 4th Parameter
    lcdWriteParameter(0x05); // 5th Parameter
    lcdWriteParameter(0x00); // 6th Parameter
    lcdWriteParameter(0x15); // 7th Parameter
    lcdWriteParameter(0xa7); // 8th Parameter
    lcdWriteParameter(0x3d); // 9th Parameter
    lcdWriteParameter(0x18); // 10th Parameter
    lcdWriteParameter(0x25); // 11th Parameter
    lcdWriteParameter(0x2a); // 12th Parameter
    lcdWriteParameter(0x2b); // 13th Parameter
    lcdWriteParameter(0x2b); // 14th Parameter
    lcdWriteParameter(0x3a); // 15th Parameter

    lcdWriteCommand(FRAME_RATE_CONTROL1);
    lcdWriteParameter(0x08); // DIVA = 8
    lcdWriteParameter(0x08); // VPA = 8

    lcdWriteCommand(DISPLAY_INVERSION);
    lcdWriteParameter(0x07); // NLA = 1, NLB = 1, NLC = 1 (all on Frame Inversion)

    lcdWriteCommand(POWER_CONTROL1);
    lcdWriteParameter(0x0a); // VRH = 10:  GVDD = 4.30
    lcdWriteParameter(0x02); // VC = 2: VCI1 = 2.65

    lcdWriteCommand(POWER_CONTROL2);
    lcdWriteParameter(0x02); // BT = 2: AVDD = 2xVCI1, VCL = -1xVCI1, VGH = 5xVCI1, VGL = -2xVCI1

    lcdWriteCommand(VCOM_CONTROL1);
    lcdWriteParameter(0x50); // VMH = 80: VCOMH voltage = 4.5
    lcdWriteParameter(0x5b); // VML = 91: VCOML voltage = -0.225

    lcdWriteCommand(VCOM_OFFSET_CONTROL);
    lcdWriteParameter(0x40); // nVM = 0, VMF = 64: VCOMH output = VMH, VCOML output = VML

    lcdWriteCommand(SET_COLUMN_ADDRESS);
    lcdWriteParameter(0x00); // XSH
    lcdWriteParameter(0x00); // XSL
    lcdWriteParameter(0x00); // XEH
    lcdWriteParameter(0x7f); // XEL (128 pixels x)

    lcdWriteCommand(SET_PAGE_ADDRESS);
    lcdWriteParameter(0x00);
    lcdWriteParameter(0x00);
    lcdWriteParameter(0x00);
    lcdWriteParameter(0x7f); // 128 pixels y

	// Select display orientation
    lcdWriteCommand(SET_ADDRESS_MODE);
	lcdWriteParameter(orientation);

	// Set the display to on
    lcdWriteCommand(SET_DISPLAY_ON);
    lcdWriteCommand(WRITE_MEMORY_START);

    lcdClearDisplay(decodeRgbValue(0, 0, 0));
}

// LCD graphics functions -----------------------------------------------------------------------------------

void lcdClearDisplay(uint16_t colour)
{
	uint16_t pixel;

	// Set the column address to 0-127
	lcdWriteCommand(SET_COLUMN_ADDRESS);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0x7f);

	// Set the page address to 0-127
	lcdWriteCommand(SET_PAGE_ADDRESS);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0x7f);

	// Plot the pixels
	lcdWriteCommand(WRITE_MEMORY_START);
	for(pixel = 0; pixel < 16385; pixel++)
	{
		lcdWriteData(colour >> 8, colour);
	}
}

void lcdPlot(uint8_t x, uint8_t y, uint16_t colour)
{
	// Horizontal Address Start Position
	lcdWriteCommand(SET_COLUMN_ADDRESS);
	lcdWriteParameter(0x00);
	lcdWriteParameter(x);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0x7f);

	// Vertical Address end Position
	lcdWriteCommand(SET_PAGE_ADDRESS);
	lcdWriteParameter(0x00);
	lcdWriteParameter(y);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0x7f);//7f

	// Plot the point
	lcdWriteCommand(WRITE_MEMORY_START);
	lcdWriteData(colour >> 8, colour);
}

// Draw a line from x0, y0 to x1, y1
// Note:	This is a version of Bresenham's line drawing algorithm
//			It only draws lines from left to right!
void lcdLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour)
{
	int16_t dy = y1 - y0;
	int16_t dx = x1 - x0;
	int16_t stepx, stepy;

	if (dy < 0)
	{
		dy = -dy; stepy = -1;
	}
	else stepy = 1;

 	if (dx < 0)
	{
		dx = -dx; stepx = -1;
	}
	else stepx = 1;

	dy <<= 1; 							// dy is now 2*dy
	dx <<= 1; 							// dx is now 2*dx

	lcdPlot(x0, y0, colour);

	if (dx > dy) {
		int fraction = dy - (dx >> 1);	// same as 2*dy - dx
		while (x0 != x1)
		{
			if (fraction >= 0)
			{
				y0 += stepy;
				fraction -= dx; 		// same as fraction -= 2*dx
			}

   			x0 += stepx;
   			fraction += dy; 				// same as fraction -= 2*dy
   			lcdPlot(x0, y0, colour);
		}
	}
	else
	{
		int fraction = dx - (dy >> 1);
		while (y0 != y1)
		{
			if (fraction >= 0)
			{
				x0 += stepx;
				fraction -= dy;
			}

			y0 += stepy;
			fraction += dx;
			lcdPlot(x0, y0, colour);
		}
	}
}

// Draw a rectangle between x0, y0 and x1, y1
void lcdRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour)
{
	lcdLine(x0, y0, x0, y1, colour);
	lcdLine(x0, y1, x1, y1, colour);
	lcdLine(x1, y0, x1, y1, colour);
	lcdLine(x0, y0, x1, y0, colour);
}

// Draw a filled rectangle
// Note:	y1 must be greater than y0  and x1 must be greater than x0
//			for this to work
void lcdFilledRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour)
{
	uint16_t pixels;

	// To speed up plotting we define a x window with the width of the
	// rectangle and then just output the required number of bytes to
	// fill down to the end point

	lcdWriteCommand(SET_COLUMN_ADDRESS); // Horizontal Address Start Position
	lcdWriteParameter(0x00);
	lcdWriteParameter(x0);
	lcdWriteParameter(0x00);
	lcdWriteParameter(x1);

	lcdWriteCommand(SET_PAGE_ADDRESS); // Vertical Address end Position
	lcdWriteParameter(0x00);
	lcdWriteParameter(y0);
	lcdWriteParameter(0x00);
	lcdWriteParameter(y1);

	lcdWriteCommand(WRITE_MEMORY_START);

	for (pixels = 0; pixels < ((x1 - x0) * (y1 - y0)); pixels++)
		lcdWriteData(colour >> 8, colour);;
}

// Draw a circle
// Note:	This is another version of Bresenham's line drawing algorithm.
//			There's plenty of documentation on the web if you are curious
//			how this works.
void lcdCircle(int16_t xCentre, int16_t yCentre, int16_t radius, uint16_t colour)
{
	int16_t x = 0, y = radius;
	int16_t d = 3 - (2 * radius);

    while(x <= y)
	{
		lcdPlot(xCentre + x, yCentre + y, colour);
		lcdPlot(xCentre + y, yCentre + x, colour);
		lcdPlot(xCentre - x, yCentre + y, colour);
		lcdPlot(xCentre + y, yCentre - x, colour);
		lcdPlot(xCentre - x, yCentre - y, colour);
		lcdPlot(xCentre - y, yCentre - x, colour);
		lcdPlot(xCentre + x, yCentre - y, colour);
		lcdPlot(xCentre - y, yCentre + x, colour);

		if (d < 0) d += (4 * x) + 6;
		else
		{
			d += (4 * (x - y)) + 10;
			y -= 1;
		}

		x++;
	}
}

void lcdDrawHeart(int16_t xCentre, int16_t yCentre, uint16_t size, uint16_t colour) {
	int i, j;
	int y = yCentre; // Start from the given yCentre
	int width = size; // Adjust width for a narrower shape

	// Upper part of the heart
	for (i = 1; i <= size / 3; i++) {
		for (j = 1; j <= 2 * width; j++) {
			if ((j >= width / 2 - i && j <= width / 2 + i) || (j >= width + width / 2 - i && j <= width + width / 2 + i)) {
				// Convert j to x-coordinate relative to xCentre
				lcdPlot(xCentre + (j - width), y, colour); // Shift x to center horizontally
			}
		}
		y++; // Move to the next row
	}

	// Lower part of the heart
	for (i = 1; i <= size; i++) {
		for (j = 1; j <= 2 * width; j++) {
			if (j >= i && j <= 2 * width - i) {
				// Convert j to x-coordinate relative to xCentre
				lcdPlot(xCentre + (j - width), y, colour); // Shift x to center horizontally
			}
		}
		y++; // Move to the next row
	}
}


// LCD text manipulation functions --------------------------------------------------------------------------
#define pgm_read_byte_near(address_short) (uint16_t)(address_short)
// Plot a character at the specified x, y co-ordinates (top left hand corner of character)
void lcdPutCh(unsigned char character, uint8_t x, uint8_t y, uint16_t fgColour, uint16_t bgColour)
{
	uint8_t row, column;

	// To speed up plotting we define a x window of 6 pixels and then
	// write out one row at a time.  This means the LCD will correctly
	// update the memory pointer saving us a good few bytes

	lcdWriteCommand(SET_COLUMN_ADDRESS); // Horizontal Address Start Position
	lcdWriteParameter(0x00);
	lcdWriteParameter(x);
	lcdWriteParameter(0x00);
	lcdWriteParameter(x+5);

	lcdWriteCommand(SET_PAGE_ADDRESS); // Vertical Address end Position
	lcdWriteParameter(0x00);
	lcdWriteParameter(y);
	lcdWriteParameter(0x00);
	lcdWriteParameter(0x7f);

	lcdWriteCommand(WRITE_MEMORY_START);

	// Plot the font data
	for (row = 0; row < 8; row++)
	{
		for (column = 0; column < 6; column++)
		{
			//if ((font5x8[character][column]) & (1 << row))
			if ((fontus[character][column]) & (1 << row))
				lcdWriteData(fgColour>>8, fgColour);
			else lcdWriteData(bgColour >> 8, bgColour);
		}
	}
}

// Translates a 3 byte RGB value into a 2 byte value for the LCD (values should be 0-31)
uint16_t decodeRgbValue(uint8_t r, uint8_t g, uint8_t b)
{
	return (b << 11) | (g << 6) | (r);
}

// This routine takes a row number from 0 to 20 and
// returns the x coordinate on the screen (0-127) to make
// it easy to place text
uint8_t lcdTextX(uint8_t x) { return x*6; }

// This routine takes a column number from 0 to 20 and
// returns the y coordinate on the screen (0-127) to make
// it easy to place text
uint8_t lcdTextY(uint8_t y) { return y*8; }

// Plot a string of characters to the LCD
void lcdPutS(const char *string, uint8_t x, uint8_t y, uint16_t fgColour, uint16_t bgColour, size_t delay)
{
	uint8_t origin = x;
	uint8_t characterNumber;

	for (characterNumber = 0; characterNumber < strlen(string); characterNumber++)
	{
		if (!checkBoundries(&x, &y, origin)) break;

		lcdPutCh(string[characterNumber], x, y, fgColour, bgColour);
		LL_mDelay(delay > 0 ? delay : 1);
		x += 6;
	}
}

void lcdPutSWithCursor(const char *string, uint8_t x, uint8_t y, uint16_t fgColour, uint16_t bgColour, size_t delay) {
    uint8_t origin = x;
    uint8_t characterNumber;

    for (characterNumber = 0; characterNumber < strlen(string); characterNumber++) {
        if (!checkBoundries(&x, &y, origin)) break;

        lcdPutCh(string[characterNumber], x, y, fgColour, bgColour);
        LL_mDelay(delay > 0 ? delay : 1);

        lcdPutCh('_', x + 6, y, fgColour, bgColour);
        LL_mDelay(delay > 0 ? delay : 1);

        lcdPutCh(' ', x + 6, y, bgColour, bgColour);

        x += 6;
    }

    for (int i = 5; i > 0; i--) {
        lcdPutCh('_', x, y, fgColour, bgColour);
        LL_mDelay(delay > 0 ? delay : 1);
        lcdPutCh(' ', x, y, bgColour, bgColour);
        LL_mDelay(delay > 0 ? delay : 1);
    }
}

void lcdPutSWithMagicalWriter(const char *string, uint8_t x, uint8_t y, uint16_t fgColour, uint16_t bgColour, size_t delay) {
    uint8_t origin = x;
    uint8_t characterNumber;
    const char char_pool[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
        'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
        ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~'
    };
    size_t pool_size = sizeof(char_pool) / sizeof(char_pool[0]);

    for (characterNumber = 0; characterNumber < strlen(string); characterNumber++) {

    	if(!checkBoundries(&x, &y, origin)) break;

        char c = string[characterNumber];

        if(c == ' ') {
			lcdPutCh(c, x, y, fgColour, bgColour);
			LL_mDelay(delay > 0 ? delay : 1);
        } else {
			for (int i = 0; i < 20; i++) {
				char random_char = char_pool[rand() % pool_size];
				lcdPutCh(random_char, x, y, fgColour, bgColour);
				LL_mDelay(5);
			}

			lcdPutCh(c, x, y, fgColour, bgColour);
			LL_mDelay(delay > 0 ? delay : 1);
        }

        x += 6;
    }

    for (int i = 0; i < 5; i++) {
        lcdPutCh('_', x, y, fgColour, bgColour);
        LL_mDelay(delay > 0 ? delay : 1);
        lcdPutCh(' ', x, y, bgColour, bgColour);
        LL_mDelay(delay > 0 ? delay : 1);
    }
}

uint8_t checkBoundries(uint8_t *x, uint8_t *y, uint8_t origin) {
	if (*x > 121) {
		*x = origin;
		*y += 8;
	}

	if (*y > 120) return 0;

	return 1;
}

