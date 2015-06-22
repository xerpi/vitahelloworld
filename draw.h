/*
 * Copyright (c) 2015 Sergi Granell (xerpi)
 */

#ifndef DRAW_H
#define DRAW_H

#include <psp2/types.h>

#define RGBA8(r, g, b, a)      ((((a)&0xFF)<<24) | (((b)&0xFF)<<16) | (((g)&0xFF)<<8) | (((r)&0xFF)<<0))

#define SCREEN_W 960
#define SCREEN_H 544

void init_video(SceDisplayFrameBuf *fb);
void end_video(SceDisplayFrameBuf *fb);
void draw_pixel(SceDisplayFrameBuf *fb, uint32_t x, uint32_t y, uint32_t color);
void draw_rectangle(SceDisplayFrameBuf *fb, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);

#define clear_screen(fb) draw_rectangle(fb, 0, 0, SCREEN_W, SCREEN_H, RGBA8(255, 255, 255, 255))

#endif
