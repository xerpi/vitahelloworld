/*
 * Copyright (c) 2015 Sergi Granell (xerpi)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include <psp2/display.h>
#include <psp2/gxm.h>
#include <psp2/types.h>
#include <psp2/moduleinfo.h>

#include "defines.h"
#include "draw.h"

int foo = 0;

int _start()
{
	SceDisplayFrameBuf fb;
	SceCtrlData pad;
	int ret;

	init_video(&fb);

	#define SQUARE_SIZE 60
	int x = SCREEN_W/2 - SQUARE_SIZE/2;
	int y = SCREEN_H/2 - SQUARE_SIZE/2;
	int inc_x = 7;
	int inc_y = 5;
	while (1) {
		clear_screen(&fb);
		//ret = sceCtrlPeekBufferPositive(0, &pad, 1);
		printf("foo: %i\n", foo++);


		/* Move the square */
		x += inc_x;
		y += inc_y;

		/* Check left and right collisions */
		if (x < 0) {
			x = 0;
			inc_x = -inc_x;
		} else if ((x + SQUARE_SIZE) > SCREEN_W) {
			x = SCREEN_W - SQUARE_SIZE;
			inc_x = -inc_x;
		}

		/* Check top and bottom collisions */
		if (y < 0) {
			y = 0;
			inc_y = -inc_y;
		} else if ((y + SQUARE_SIZE) > SCREEN_H) {
			y = SCREEN_H - SQUARE_SIZE;
			inc_y = -inc_y;
		}

		/* Draw the square */
		draw_rectangle(&fb, x, y, SQUARE_SIZE, SQUARE_SIZE, RGBA8(255, 0, 0, 255));

		sceDisplayWaitVblankStart();
	}

	end_video(&fb);
	return 0;
}
