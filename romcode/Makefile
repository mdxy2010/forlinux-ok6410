CC = arm-none-linux-gnueabi-gcc
LD = arm-none-linux-gnueabi-ld
OBJCOPY = arm-none-linux-gnueabi-objcopy
LDPATH=-lgcc -L $(shell dirname `$(CC) -print-libgcc-file-name`)
LDFLAGS=-Tgboot.lds -nostartfiles

all: start.o main.o mmu.o led.o button.o interrupt.o mem.o nand.o uart.o console.o vsprintf.o string.o ctype.o div64.o
	$(LD) $(LDFLAGS) -o gboot.elf $^ $(LDPATH)
	$(OBJCOPY) -O binary gboot.elf gboot.bin
%.o : %.S
	$(CC) -g -c $^
	
%.o : %.c
	$(CC) -g -fno-builtin -I ./include -c $^
	
.PHONY: clean
clean:
	rm *.o *.elf *.bin


