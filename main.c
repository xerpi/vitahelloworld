/*
 * Copyright (c) 2015 Sergi Granell (xerpi)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/time.h>

#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include <psp2/display.h>
#include <psp2/gxm.h>
#include <psp2/types.h>
#include <psp2/moduleinfo.h>
#include <psp2/kernel/processmgr.h>

#include "utils.h"
#include "draw.h"

int main()
{
	init_video();

	/* FPS counting */
	SceUInt64 cur_micros = 0, delta_micros = 0, last_micros = 0;
	uint32_t frames = 0;
	float fps = 0.0f;

	/* Square variables */
	int w = 60;
	int h = 60;
	int x = SCREEN_W/2 - w/2;
	int y = SCREEN_H/2 - h/2;
	int speed = 2;
	uint32_t color = RGBA8(255, 0, 0, 255);

	/* Input variables */
	SceCtrlData pad;
	SceTouchData touch;
	signed char lx, ly, rx, ry;

	while (1) {
		clear_screen();

		/* Read controls and touchscreen */
		sceCtrlPeekBufferPositive(0, &pad, 1);
		sceTouchPeek(0, &touch, 1);

		if (pad.buttons & SCE_CTRL_START) {
			break;
		}

		font_draw_string(10, 10, RGBA8(0, 0, 255, 255), "PSVita sample by xerpi!");
		font_draw_stringf(SCREEN_W - 160, 10, RGBA8(0, 255, 0, 255), "FPS: %.2f", fps);
		font_draw_stringf(10, 30, RGBA8(255, 0, 0, 255),
			"(%3d, %3d) size: (%d, %d) speed: %d\n", x, y, w, h, speed);

		/* Move the rectangle */
		if (pad.buttons & SCE_CTRL_UP) {
			y -= speed;
		} else 	if (pad.buttons & SCE_CTRL_DOWN) {
			y += speed;
		}
		if (pad.buttons & SCE_CTRL_LEFT) {
			x -= speed;
		} else 	if (pad.buttons & SCE_CTRL_RIGHT) {
			x += speed;
		}

		if (pad.buttons & SCE_CTRL_LTRIGGER) {
			speed--;
			if (speed < 0) speed = 0;
		} else 	if (pad.buttons & SCE_CTRL_RTRIGGER) {
			speed++;
			if (speed > 100) speed = 100;
		}

		if (pad.buttons & SCE_CTRL_CROSS) {
			color = RGBA8(rand()%255, rand()%255, rand()%255, 255);
		}

		/* Update joystick values */
		lx = (signed char)pad.lx - 128;
		ly = (signed char)pad.ly - 128;
		rx = (signed char)pad.rx - 128;
		ry = (signed char)pad.ry - 128;

		/* Move using the left yoystick */
		if (abs(lx) > 50) {
			x += speed * lx/50.0f;
		}
		if (abs(ly) > 50) {
			y += speed * ly/50.0f;
		}

		/* Resize using the right yoystick */
		if (abs(rx) > 50) {
			w += rx/15.0f;
			if (w < 5) {
				w = 5;
			} else if (w > SCREEN_W) {
				w = SCREEN_W;
			}
		}
		if (abs(ry) > 50) {
			h += ry/15.0f;
			if (h < 5) {
				h = 5;
			} if (h > SCREEN_H) {
				h = SCREEN_H;
			}
		}

		/* Move using the touchscreen! */
		if (touch.reportNum > 0) {
			/* Front touchscreen: 1920x1088 */
			x = lerp(touch.report[0].x, 1920, SCREEN_W) - w/2;
			y = lerp(touch.report[0].y, 1088, SCREEN_H) - h/2;
		}

		/* Check left and right collisions */
		if (x < 0) {
			x = 0;
		} else if ((x + w) > SCREEN_W) {
			x = SCREEN_W - w;
		}

		/* Check top and bottom collisions */
		if (y < 0) {
			y = 0;
		} else if ((y + h) > SCREEN_H) {
			y = SCREEN_H - h;
		}

		/* Draw the rectangle */
		draw_rectangle(x, y, w, h, color);

		/* Draw a circle */
		draw_circle(SCREEN_W / 2, SCREEN_H / 2, 50, RGBA8(0,0,255,255));

		/* Calculate FPS */
		cur_micros = sceKernelGetProcessTimeWide();

		if (cur_micros >= (last_micros + 1000000)) {
			delta_micros = cur_micros - last_micros;
			last_micros = cur_micros;
			fps = (frames/(double)delta_micros)*1000000.0f;
			frames = 0;
		}

		swap_buffers();
		sceDisplayWaitVblankStart();
		frames++;
	}

	end_video();
	sceKernelExitProcess(0);
	return 0;
}
