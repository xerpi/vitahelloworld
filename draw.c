/*
 * Copyright (c) 2015 Sergi Granell (xerpi)
 */

#include <stdio.h>
#include <stdarg.h>
#include <psp2/display.h>
#include <psp2/gxm.h>
#include "draw.h"
#include "defines.h"

extern const unsigned char msx_font[];

static SceDisplayFrameBuf fb;

static void *alloc_gpu_mem(uint32_t type, uint32_t size, uint32_t attribs, SceUID *uid)
{
	int ret;
	void *mem = NULL;

	if (type == SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW)
		size = align_mem(size, 256 * 1024);
	else
		size = align_mem(size, 4 * 1024);

	*uid = sceKernelAllocMemBlock("gxm", type, size, NULL);

	printf("MemBlock uid: 0x%08X\n", *uid);

	ret = sceKernelGetMemBlockBase(*uid, &mem);
	printf("sceKernelGetMemBlockBase(): 0x%08X\n", ret);
	printf("MemBlockBase addr: %p\n", mem);
	if (ret != 0) {
		return NULL;
	}

	ret = sceGxmMapMemory(mem, size, attribs);
	printf("sceGxmMapMemory(): 0x%08X\n", ret);
	if (ret != 0) {
		return NULL;
	}

	return mem;
}

void init_video()
{
	int ret;
	SceUID uid;

	SceGxmInitializeParams params;

	params.flags                        = 0x0;
	params.displayQueueMaxPendingCount  = 0x0;
	params.displayQueueCallback         = 0x0;
	params.displayQueueCallbackDataSize = 0x0;
	params.parameterBufferSize          = (16 * 1024 * 1024);

	/* Initialize the GXM */
	ret = sceGxmInitialize(&params);
	printf("sceGxmInitialize(): 0x%08X\n", ret);

	/* Get current buffer info */
	fb.size = sizeof(fb);
	ret = sceDisplayGetFrameBuf(&fb, PSP2_DISPLAY_SETBUF_IMMEDIATE);
	printf("sceDisplayGetFrameBuf(): 0x%08X\n", ret);

	/* Allocate a new framebuffer */
	void *base = alloc_gpu_mem(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW, 4*SCREEN_W*SCREEN_H, SCE_GXM_MEMORY_ATTRIB_RW, &uid);
	if (base == NULL) {
		printf("Could not allocate memory. %p", base);
		return;
	}

	/* Use the new framebuffer! */
	fb.base = base;
	sceDisplaySetFrameBuf(&fb, PSP2_DISPLAY_SETBUF_NEXTFRAME);

	printf(
		"\nframebuffer:\n"
		"\tsize:           0x%08X\n"
		"\tbase:           0x%08X\n"
		"\tpitch:          0x%08X\n"
		"\tpixelformat:    0x%08X\n"
		"\twidth:          0x%08X\n"
		"\theight          0x%08X\n",
		fb.size, (uintptr_t)fb.base,
		fb.pitch, fb.pixelformat, fb.width, fb.height);
}

void end_video()
{
	sceGxmUnmapMemory(fb.base);
	sceGxmTerminate();
}

void draw_pixel(uint32_t x, uint32_t y, uint32_t color)
{
	((uint32_t *)fb.base)[x + y*fb.pitch] = color;
}

void draw_rectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
{
	int i, j;
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			((uint32_t *)fb.base)[(x + j) + (y + i)*fb.pitch] = color;
		}
	}
}

void font_draw_char(int x, int y, uint32_t color, char c)
{
	unsigned char *font = (unsigned char *)(msx_font + (c - (uint32_t)' ') * 8);
	int i, j, pos_x, pos_y;
	for (i = 0; i < 8; ++i) {
		pos_y = y + i*2;
		for (j = 0; j < 8; ++j) {
			pos_x = x + j*2;
			if ((*font & (128 >> j))) {
				draw_pixel(pos_x + 0, pos_y + 0, color);
				draw_pixel(pos_x + 1, pos_y + 0, color);
				draw_pixel(pos_x + 0, pos_y + 1, color);
				draw_pixel(pos_x + 1, pos_y + 1, color);
			}
		}
		++font;
	}
}

void font_draw_string(int x, int y, uint32_t color, const char *string)
{
	if (string == NULL) return;

	int startx = x;
	const char *s = string;

	while (*s) {
		if (*s == '\n') {
			x = startx;
			y += 16;
		} else if (*s == ' ') {
			x += 16;
		} else if(*s == '\t') {
			x += 16*4;
		} else {
			font_draw_char(x, y, color, *s);
			x += 16;
		}
		++s;
	}
}

void font_draw_stringf(int x, int y, uint32_t color, const char *s, ...)
{
	char buf[256];
	va_list argptr;
	va_start(argptr, s);
	vsnprintf(buf, sizeof(buf), s, argptr);
	va_end(argptr);
	font_draw_string(x, y, color, buf);
}

