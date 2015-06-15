/*
 *  lcdpcf8574test.cpp:
 *  Test program for I²C based PCF8574 driver for HD44780 based LCD
 *  to be used with wiringPi for Raspberry Pi
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

#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime> 
#include <unistd.h>
#include "lcdpcf8574.h"

using namespace std;

int volatile keepRunning = 1;

void intHandler(int dummy) {
    keepRunning = 0;
    printf("intHandler called\n");
}

void show_time(lcd& m)
{
	char buf[64];

		time_t tim = time (NULL);
		tm* t = localtime (&tim);

		sprintf (buf, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec) ;
		m.putsAt(buf, 0, 0);

		sprintf (buf, "%02d/%02d/%04d", t->tm_mday, t->tm_mon + 1, t->tm_year+1900) ;
		m.putsAt(buf, 1,  0);
}



int main(int argc, char *argv[]) {
	signal(SIGINT, intHandler);
	lcd m(0x20);
	uint8_t customfonts[8][8] = {
			 {0b00001110, 0b00011011, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00011111}
			,{0b00001110, 0b00011011, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00011111, 0b00011111}
			,{0b00001110, 0b00011011, 0b00010001, 0b00010001, 0b00010001, 0b00011111, 0b00011111, 0b00011111}
			,{0b00001110, 0b00011011, 0b00010001, 0b00010001, 0b00011111, 0b00011111, 0b00011111, 0b00011111}
			,{0b00001110, 0b00011011, 0b00010001, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111}
			,{0b00001110, 0b00011011, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111}
			,{0b00001110, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111}
			,{0b00001110, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111}
	};

	m.loadcustomfonts(customfonts);
	m.putsAt("hello", 0, 0);
	sleep(5);

	m.putAt(0x04, 0, 15);
	for(;keepRunning == 1;)
	{
		show_time(m);
		usleep(500);
	}
	m.clear();

}
