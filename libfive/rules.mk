PREFIX=riscv64-unknown-elf

RV_ROOT ?= $(RISCV)

CC=$(PREFIX)-gcc
AS=$(PREFIX)-as
LD=$(PREFIX)-ld
OBJCOPY=$(PREFIX)-objcopy
OBJDUMP=$(PREFIX)-objdump

INCLUDE=-I$(LIBFIVE_ROOT)/..
ARCH=rv64g


O ?= s

CFLAGS=-O$(O) $(INCLUDE) -g -Wall -Wno-unused-function -nostdlib -nostartfiles -ffreestanding -march=$(ARCH) -mabi=lp64 -std=gnu99 -mcmodel=medany
ASFLAGS=-march=$(ARCH) -mabi=lp64
LDFLAGS=-T $(LIBFIVE_ROOT)/memmap.ld -melf64lriscv
LDLIBS=

LIBCSRC=$(wildcard $(LIBFIVE_ROOT)/*.c) $(wildcard $(LIBFIVE_ROOT)/libc/*.c)
LIBSSRC=$(wildcard $(LIBFIVE_ROOT)/*.s)
LIBOBJ=$(LIBCSRC:.c=.o) $(LIBSSRC:.s=.o)

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(PROG).hex $(PROG).list

install: $(PROG).hex
	@test -n "$(RISCV_MOUNT)" || (echo "Set the RISCV_MOUNT environment variable to the path at which your HiFive1 RevB board is mounted. This will probably be under /media or /Volumes." && false)
	mv $< $(RISCV_MOUNT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -c -o $@

$(PROG).elf: $(OBJ) $(LIBOBJ)
	$(LD) $(LDFLAGS) $(LIBOBJ) $(OBJ) $(LDLIBS) -o $@

%.bin: %.elf
	$(OBJCOPY) $< -O binary $@

%.hex: %.elf
	$(OBJCOPY) $< -O ihex $@

%.list: %.elf
	$(OBJDUMP) -D $< > $@

clean:
	rm -f *.o *.elf *.hex *.bin *.list
	rm -f $(LIBFIVE_ROOT)/*.o
	rm -f $(LIBFIVE_ROOT)/libc/*.o

.PHONY: all install clean
