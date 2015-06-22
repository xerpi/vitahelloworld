/*
 * Copyright (c) 2015 Sergi Granell (xerpi)
 */

#ifndef DRAW_H
#define DRAW_H

#include <psp2/types.h>

#define RGBA8(r, g, b, a)      ((((a)&0xFF)<<24) | (((b)&0xFF)<<16) | (((g)&0xFF)<<8) | (((r)&0xFF)<<0))

#define SCREEN_W 960
#define SCREEN_H 544

void init_video();
void end_video();
void swap_buffers();
void draw_pixel(uint32_t x, uint32_t y, uint32_t color);
void draw_rectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);

void font_draw_char(int x, int y, uint32_t color, char c);
void font_draw_string(int x, int y, uint32_t color, const char *string);
void font_draw_stringf(int x, int y, uint32_t color, const char *s, ...);

#define clear_screen() draw_rectangle(0, 0, SCREEN_W, SCREEN_H, RGBA8(255, 255, 255, 255))

#endif
