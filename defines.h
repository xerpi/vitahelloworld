/*
 * Copyright (c) 2015 Sergi Granell (xerpi)
 */

#ifndef DEFINES_H
#define DEFINES_H

#include <psp2/types.h>

#define align_mem(addr, align) (((addr) + ((align) - 1)) & ~((align) - 1))

extern int sceKernelAllocMemBlock(const char *name, int type, int size, void *optp);
extern int sceKernelGetMemBlockBase(SceUID uid, void **basep);


/* grabbed from: Hykem/vitasploit/include/FW_200_320/functions_ex.js */

#define SCE_OK 0x0

/* SceKernel */

#define SCE_KERNEL_MEMBLOCK_TYPE_USER_RW			0x0c20d060
#define SCE_KERNEL_MEMBLOCK_TYPE_USER_RW_UNCACHE		0x0c208060
#define SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_PHYCONT_RW		0x0c80d060
#define SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_PHYCONT_NC_RW	0x0d808060
#define SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW			0x09408060

/* SceGxm */

#define SCE_GXM_MEMORY_ATTRIB_READ	0x1
#define SCE_GXM_MEMORY_ATTRIB_WRITE	0x2
#define SCE_GXM_MEMORY_ATTRIB_RW	(SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE)

/* SceCtrl */

#define SCE_CTRL_SELECT				0x00001
#define SCE_CTRL_START				0x00008
#define SCE_CTRL_UP				0x00010
#define SCE_CTRL_RIGHT				0x00020
#define SCE_CTRL_DOWN				0x00040
#define SCE_CTRL_LEFT				0x00080
#define SCE_CTRL_LTRIGGER			0x00100
#define SCE_CTRL_RTRIGGER			0x00200
#define SCE_CTRL_TRIANGLE			0x01000
#define SCE_CTRL_CIRCLE				0x02000
#define SCE_CTRL_CROSS				0x04000
#define SCE_CTRL_SQUARE				0x08000
#define SCE_CTRL_INTERCEPTED			0x10000
#define SCE_CTRL_MODE_DIGITALONLY		0x0
#define SCE_CTRL_MODE_DIGITALANALOG		0x1
#define SCE_CTRL_MODE_DIGITALANALOG_WIDE	0x2

/* SceTouch */

#define SCE_TOUCH_PORT_FRONT	0x0
#define SCE_TOUCH_PORT_BACK	0x1

#endif
