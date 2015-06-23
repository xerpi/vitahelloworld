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

typedef struct {
	unsigned int 	timeStamp;
	unsigned int	unk0;
	unsigned int 	buttons;
	unsigned char 	lx;
	unsigned char 	ly;
	unsigned char 	rx;
	unsigned char 	ry;
	uint8_t 	reserved[16];
} CtrlData;


#endif
