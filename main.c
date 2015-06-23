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

int _start()
{
	init_video();

	int size = 60;
	int x = SCREEN_W/2 - size/2;
	int y = SCREEN_H/2 - size/2;
	int speed = 2;
	uint32_t color = RGBA8(255, 0, 0, 255);

	CtrlData pad;
	SceTouchData touch;

	while (1) {
		clear_screen();

		sceCtrlPeekBufferPositive(0, (SceCtrlData *)&pad, 1);
		sceTouchPeek(0, &touch, 1);

		font_draw_string(10, 10, RGBA8(0, 0, 255, 255), "PSVita sample by xerpi!");
		font_draw_stringf(10, 30, RGBA8(0, 0, 255, 255),
			"(%3d, %3d) size: %d speed: %d\n", x, y, size, speed);

		/* Move the square */
		if (pad.buttons & PSP2_CTRL_UP) {
			y -= speed;
		} else 	if (pad.buttons & PSP2_CTRL_DOWN) {
			y += speed;
		}
		if (pad.buttons & PSP2_CTRL_LEFT) {
			x -= speed;
		} else 	if (pad.buttons & PSP2_CTRL_RIGHT) {
			x += speed;
		}

		if (pad.buttons & PSP2_CTRL_SQUARE) {
			speed--;
			if (speed < 0) speed = 0;
		} else 	if (pad.buttons & PSP2_CTRL_CIRCLE) {
			speed++;
			if (speed > 100) speed = 100;
		}

		if (pad.buttons & PSP2_CTRL_LTRIGGER) {
			size--;
			if (size < 0) size = 0;
		} else 	if (pad.buttons & PSP2_CTRL_RTRIGGER) {
			size++;
			if (size > SCREEN_H) size = SCREEN_H;
		}

		if (pad.buttons & PSP2_CTRL_CROSS) {
			color = RGBA8(rand()%255, rand()%255, rand()%255, 255);
		}

		/* Move using the touchscreen! */
		if (touch.reportNum > 0) {
			/* Front touchscreen: 1920x1088 */
			x = lerp(touch.report[0].x, 1920, SCREEN_W) - size/2;
			y = lerp(touch.report[0].y, 1088, SCREEN_H) - size/2;
		}

		/* Check left and right collisions */
		if (x < 0) {
			x = 0;
		} else if ((x + size) > SCREEN_W) {
			x = SCREEN_W - size;
		}

		/* Check top and bottom collisions */
		if (y < 0) {
			y = 0;
		} else if ((y + size) > SCREEN_H) {
			y = SCREEN_H - size;
		}

		/* Draw the square */
		draw_rectangle(x, y, size, size, color);

		swap_buffers();
		sceDisplayWaitVblankStart();
	}

	end_video();
	return 0;
}
