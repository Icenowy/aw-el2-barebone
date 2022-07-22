CROSS_COMPILE ?= /opt/abcross/arm64/bin/aarch64-aosc-linux-gnu-

DEBUG ?= 0

CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

INCLUDES = -I ./include -I ./uboot-headers -ffreestanding

DEFINES = -DSOC_HEADER="\"h6.h\""

ifneq ($(DEBUG),1)
DEFINES += -DNDEBUG
endif

ASFLAGS = $(INCLUDES) $(DEFINES) -D__ASSEMBLY__
CFLAGS = $(INCLUDES) $(DEFINES) -O2 -mno-outline-atomics -fno-stack-protector

LDSCRIPT = ldscripts/a64.ld
LDSCRIPTS = ldscripts/a64.ld ldscripts/common.ld

LDFLAGS = -nostdlib -static -T $(LDSCRIPT)

OBJS = start.o init.o uart.o stack.o exceptions.o exception_funcs.o panic.o pgtables.o trapped_funcs.o

all: el2-bb.bin

el2-bb.elf: $(OBJS) $(LDSCRIPTS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

el2-bb.bin: el2-bb.elf
	$(OBJCOPY) -O binary $< $@

.PHONY: clean
clean:
	rm -f el2-bb.* *.o
