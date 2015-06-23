#
# Copyright (c) 2015 Sergi Granell (xerpi)
# based on Cirne's vita-toolchain test Makefile
#

TARGET = vitahelloworld
OBJS   = main.o draw.o font_data.o

STUBS = libSceLibKernel.a libSceDisplay.a libSceGxm.a libSceSysmem.a \
	libSceCtrl.a libSceTouch.a

NIDS_DB = sample-db.json

PREFIX  = $(DEVKITARM)/bin/arm-none-eabi
CC      = $(PREFIX)-gcc
READELF = $(PREFIX)-readelf
OBJDUMP = $(PREFIX)-objdump
CFLAGS  = -Wall -nostartfiles -nostdlib -I$(PSP2SDK)/include

STUBS_FULL = $(addprefix stubs/, $(STUBS))

all: $(TARGET).velf

$(TARGET).velf: $(TARGET).elf
	vita-elf-create $(TARGET).elf $(TARGET).velf $(NIDS_DB)
#	$(READELF) -a $(TARGET).velf
#	$(OBJDUMP) -D -j .text.fstubs $(TARGET).velf
#	$(OBJDUMP) -s -j .data.vstubs -j .sceModuleInfo.rodata -j .sceLib.ent -j .sceExport.rodata -j .sceLib.stubs -j .sceImport.rodata -j .sceFNID.rodata -j .sceFStub.rodata -j .sceVNID.rodata -j .sceVStub.rodata -j .sce.rel $(TARGET).velf

$(TARGET).elf: $(OBJS) $(STUBS_FULL)
	$(CC) -Wl,-q -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(STUBS_FULL):
	@mkdir -p stubs
	vita-libs-gen $(NIDS_DB) stubs/
	$(MAKE) -C stubs $(notdir $@)

clean:
	@rm -rf $(TARGET).elf $(TARGET).velf $(OBJS) stubs

copy: $(TARGET).velf
	@cp $(TARGET).velf ~/shared/vitasample.elf
