/*
 *  lcdpcf8574.cpp:
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

#include "lcdpcf8574.h"

#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <stdexcept>

#include <wiringPiI2C.h>


// lcdpcf8574 constructor
lcd::lcd(int addr)
  : fd(wiringPiI2CSetup(addr))
  , blFlag(LCD_BACKLIGHT)
{
	if (fd < 0)
        {
		throw std::runtime_error("error initializing I2C device PCF8574");
	}

	int displayshift = (LCD_CURSORMOVE | LCD_MOVERIGHT);
	int displaymode = (LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);
	int displaycontrol = (LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);

	wiringPiI2CWrite(fd, 0x20);
	strobe();
	write_cmd(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);
	write_cmd(LCD_DISPLAYCONTROL | displaycontrol);
	write_cmd(LCD_ENTRYMODESET | displaymode);

	write_cmd(LCD_CLEARDISPLAY);
	write_cmd(LCD_CURSORSHIFT | displayshift);
	write_cmd(LCD_RETURNHOME);
}

void lcd::backlightOn(bool on)
{
blFlag = on ? LCD_BACKLIGHT : LCD_NOBACKLIGHT; 
strobe();
}

// clocks EN to latch command
void lcd::strobe() 
{
	wiringPiI2CWrite(fd, wiringPiI2CRead(fd) | EN );
	wiringPiI2CWrite(fd, (wiringPiI2CRead(fd) ) & ~EN);
}


unsigned char highnib(unsigned char data)
{
    return (data >> 4) & 0xf;
}
unsigned char lownib(unsigned char data)
{
    return data & 0xf;
}

//write data to lcd in 4 bit mode, 2 nibbles
//high nibble is sent first
void lcd::write_cmd(uint8_t cmd) 
{
//	write high nibble
	wiringPiI2CWrite(fd, highnib(cmd) | blFlag);
	wiringPiI2CRead(fd);
	strobe();

//	then low nibble
	wiringPiI2CWrite(fd, lownib(cmd) | blFlag);
	wiringPiI2CRead(fd);
	strobe();
	wiringPiI2CWrite(fd, blFlag);
}

//write a character to lcd (or character rom) 0x09: backlight | RS=DR
//works as expected
void lcd::write_data(uint8_t charvalue)
{
	int controlFlag = blFlag | RS;
//	write high nibble
	wiringPiI2CWrite(fd, (controlFlag & 0xf0)| highnib(charvalue) );
	strobe();

//	write low nibble
	wiringPiI2CWrite(fd, (controlFlag & 0xf0) | lownib(charvalue) );
	strobe();
	wiringPiI2CWrite(fd, blFlag);
}

// load custom characters
void lcd::load_custom_font(int addr, const unsigned char font[8])
{
	write_cmd(addr*8 + LCD_SETCGRAMADDR);
	int x;
	for(x=0;x<8;x++)
	{
		write_data(font[x]);
	}
}

void lcd::_setDDRAMAddress(int line, int col)
{
  if (line <0 || line >3) throw std::runtime_error("Invalid line value");
  static unsigned char offsets[4] = { 0x00, 0x40, 0x14, 0x54};
//we write to the Data Display RAM (DDRAM)
    write_cmd(LCD_SETDDRAMADDR | (offsets[line] + col));
}

//put string function
void lcd::putsAt(const char string[20], int line, int col)
{
	_setDDRAMAddress(line, col);
	const int len = strlen(string);
	for(int i = 0; i < len; i++)
        {
	    write_data(string[i]);
	}
}

void lcd::putAt(uint8_t c, int line, int col)
{
	_setDDRAMAddress(line, col);
	write_data(c);
}


//clear lcd and set to home
void lcd::clear()
{
	write_cmd(LCD_CLEARDISPLAY);
	write_cmd(LCD_RETURNHOME);
}

// load 8 custom fonts to all available CGRAM locations
void lcd::loadcustomfonts(const unsigned char fonts[8][8]) 
{
	for (int x = 0; x < 8; x++) 
        {
		load_custom_font(x, fonts[x]);
	}
}

void lcd::loadcustomfont(const unsigned char font[8], int addr)
{
	load_custom_font(addr, font);
}


