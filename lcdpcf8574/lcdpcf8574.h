/*
 *  lcdpcf8574.h:
 *  I²C based PCF8574 driver for HD44780 based LCD to be used with wiringPi
 *  for Raspberry Pi
 *
 * Copyright (c) 2013 Prageeth Karunadheera. http://karunadheera.com/
 ***********************************************************************
 *
 *    lcdpcf8574 is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    lcdpcf8574 is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#include <stdint.h>

#ifndef LCDPCF8574_H
#define LCDPCF8574_H

// initializes objects and lcd

// LCD Commands
const unsigned int LCD_CLEARDISPLAY = 0x01;
const unsigned int LCD_RETURNHOME = 0x02;
const unsigned int LCD_ENTRYMODESET = 0x04;
const unsigned int LCD_DISPLAYCONTROL = 0x08;
const unsigned int LCD_CURSORSHIFT = 0x10;
unsigned int LCD_FUNCTIONSET = 0x20;
unsigned int LCD_SETCGRAMADDR = 0x40;
unsigned int LCD_SETDDRAMADDR = 0x80;

// Flags for display on/off control
unsigned int LCD_DISPLAYON = 0x04;
unsigned int LCD_DISPLAYOFF = 0x00;
unsigned int LCD_CURSORON = 0x02;
unsigned int LCD_CURSOROFF = 0x00;
unsigned int LCD_BLINKON = 0x01;
unsigned int LCD_BLINKOFF = 0x00;

// Flags for display entry mode
unsigned int LCD_ENTRYRIGHT = 0x00;
unsigned int LCD_ENTRYLEFT = 0x02;
unsigned int LCD_ENTRYSHIFTINCREMENT = 0x01;
unsigned int LCD_ENTRYSHIFTDECREMENT = 0x00;

// Flags for display/cursor shift
unsigned int LCD_DISPLAYMOVE = 0x08;
unsigned int LCD_CURSORMOVE = 0x00;
unsigned int LCD_MOVERIGHT = 0x04;
unsigned int LCD_MOVELEFT = 0x00;

// flags for function set
unsigned int LCD_8BITMODE = 0x10;
unsigned int LCD_4BITMODE = 0x00;
unsigned int LCD_2LINE = 0x08;
unsigned int LCD_1LINE = 0x00;
unsigned int LCD_5x10DOTS = 0x04;
unsigned int LCD_5x8DOTS = 0x00;

// flags for backlight control
// invert backlight flag since LEDs are common anode
unsigned int LCD_BACKLIGHT = 0x00;
unsigned int LCD_NOBACKLIGHT = 1 << 7;
unsigned int EN = 1 << 4;  // Enable bit
unsigned int RW = 1 << 5;  // Read/Write bit
unsigned int RS = 1 << 6;  // Register select bit


/*
pinout for 20x4 LCD via GY-IICLCD PCF8574:
----------
0x80    P7 - Backlight
0x40    P6 - RS  Register select  0-Instruction  1=Data
0x20    P5 - RW
0x10    P4 - EN  Enable (data clocked in on falling edge)  

0x08    P3 -  D3
0x04    P2 -  D2
0x02    P1 -  D1
0x01    P0 -  D0
*/


class lcd 
{
private:
	int fd;
        int blFlag;
	lcd(); // private default constructor
public:
	lcd(int addr); // public constructor with address for I²C device
	void backlightOn(bool on);
	void clear();

        void strobe();

	void write_cmd(uint8_t cmd);
	void write_data(uint8_t charvalue);

	void _setDDRAMAddress(int line, int col);
	void putsAt(const char s[20], int line, int col);
	void putAt(uint8_t c, int line, int col);

	void load_custom_font(int addr, const uint8_t font[8]);
	void loadcustomfonts(const unsigned char fonts[8][8]);
	void loadcustomfont(const unsigned char font[8], int addr);
};


#endif
