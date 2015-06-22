/*
 * Copyright (c) 2015 Sergi Granell (xerpi)
 */

#include <stdio.h>
#include <psp2/display.h>
#include <psp2/gxm.h>
#include "draw.h"
#include "defines.h"

// Global vars don't work?
//static SceDisplayFrameBuf fb;

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

void init_video(SceDisplayFrameBuf *fb)
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
	fb->size = sizeof(*fb);
	ret = sceDisplayGetFrameBuf(fb, PSP2_DISPLAY_SETBUF_IMMEDIATE);
	printf("sceDisplayGetFrameBuf(): 0x%08X\n", ret);

	/* Allocate a new framebuffer */
	void *base = alloc_gpu_mem(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW, 4*SCREEN_W*SCREEN_H, SCE_GXM_MEMORY_ATTRIB_RW, &uid);
	if (base == NULL) {
		printf("Could not allocate memory. %p", base);
		return;
	}

	/* Use the new framebuffer! */
	fb->base = base;
	sceDisplaySetFrameBuf(fb, PSP2_DISPLAY_SETBUF_NEXTFRAME);

	printf(
		"\nframebuffer:\n"
		"\tsize:           0x%08X\n"
		"\tbase:           0x%08X\n"
		"\tpitch:          0x%08X\n"
		"\tpixelformat:    0x%08X\n"
		"\twidth:          0x%08X\n"
		"\theight          0x%08X\n",
		fb->size, (uintptr_t)fb->base,
		fb->pitch, fb->pixelformat, fb->width, fb->height);
}

void end_video(SceDisplayFrameBuf *fb)
{
	sceGxmUnmapMemory(fb->base);
	sceGxmTerminate();
}

void draw_pixel(SceDisplayFrameBuf *fb, uint32_t x, uint32_t y, uint32_t color)
{
	((uint32_t *)fb->base)[x + y*fb->pitch] = color;
}

void draw_rectangle(SceDisplayFrameBuf *fb, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
{
	int i, j;
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			((uint32_t *)fb->base)[(x + j) + (y + i)*fb->pitch] = color;
		}
	}
}

