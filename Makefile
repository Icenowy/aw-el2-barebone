CROSS_COMPILE ?= /opt/abcross/arm64/bin/aarch64-aosc-linux-gnu-

CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

INCLUDES = -I $(PWD)/include -I $(PWD)/uboot-headers -ffreestanding

DEFINES = -DSOC_HEADER="\"a64.h\""

ASFLAGS = $(INCLUDES) $(DEFINES) -D__ASSEMBLY__
CFLAGS = $(INCLUDES) $(DEFINES) -O2

LDSCRIPT = ldscripts/a64.ld
LDSCRIPTS = ldscripts/a64.ld ldscripts/common.ld

LDFLAGS = -nostdlib -nostartfiles -static -T $(LDSCRIPT)

OBJS = start.o init.o uart.o

all: el2-bb.bin

el2-bb.elf: $(OBJS) $(LDSCRIPTS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

el2-bb.bin: el2-bb.elf
	$(OBJCOPY) -O binary $< $@

.PHONY: clean
clean:
	rm -f el2-bb.* *.o
