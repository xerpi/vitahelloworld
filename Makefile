#
# Copyright (c) 2015 Sergi Granell (xerpi)
# based on Cirne's vita-toolchain test Makefile
#

TARGET = vitahelloworld
OBJS   = main.o draw.o

OBJS += stubs/libSceLibKernel.a stubs/libSceDisplay.a stubs/libSceGxm.a stubs/libSceSysmem.a stubs/libSceCtrl.a

NIDS_DB = sample-db.json

PREFIX  = $(DEVKITARM)/bin/arm-none-eabi
CC      = $(PREFIX)-gcc
READELF = $(PREFIX)-readelf
OBJDUMP = $(PREFIX)-objdump
CFLAGS  = -Wall -nostartfiles -nostdlib -I$(PSP2SDK)/include

all: $(TARGET).velf

$(TARGET).velf: $(TARGET).elf
	vita-elf-create $(TARGET).elf $(TARGET).velf $(NIDS_DB)
#	$(READELF) -a $(TARGET).velf
#	$(OBJDUMP) -D -j .text.fstubs $(TARGET).velf
#	$(OBJDUMP) -s -j .data.vstubs -j .sceModuleInfo.rodata -j .sceLib.ent -j .sceExport.rodata -j .sceLib.stubs -j .sceImport.rodata -j .sceFNID.rodata -j .sceFStub.rodata -j .sceVNID.rodata -j .sceVStub.rodata -j .sce.rel $(TARGET).velf

$(TARGET).elf: $(OBJS)
	$(CC) -Wl,-q -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

stubs/%.a: stubs $(NIDS_DB)
	vita-libs-gen sample-db.json stubs/
	$(MAKE) -C stubs $*.a

stubs:
	@mkdir stubs

clean:
	@rm -rf $(TARGET).elf $(TARGET).velf $(OBJS) stubs

copy: $(TARGET).velf
	@cp $(TARGET).velf ~/shared/vitasample.elf
