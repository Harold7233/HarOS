C_SOURCES = $(wildcard common/*.c *.c x86/*.c libc/*.c mm/*.c kernel/*.c task/*.c)
ASM_SOURCES = $(wildcard asm/*.asm)
HEADERS = $(wildcard common/*.h x86/*.h libc/*.h mm/*.h task/*.h kernel/*.h)
OBJECTS = ${ASM_SOURCES:.asm=.o} loader.o ${C_SOURCES:.c=.o}
CC = i386-elf-gcc
CFLAGS = -g -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
			-nostartfiles -nodefaultlibs -Wall -Wextra -c -I. #-masm=intel
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf  os.iso
	make clean
	i386-elf-objdump -d -M intel kernel.elf > kernel.dis
	bochs -qf bochsrc

gdb: kernel.elf  os.iso
	make clean
	i386-elf-objdump -d -M intel kernel.elf > kernel.dis
	\/opt/bochs/gdbstub/bin/bochs -qf bochsrc
	
	
kernel.elf: $(OBJECTS)
	i386-elf-ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	mkisofs -R                              \
				-b boot/grub/stage2_eltorito    \
				-no-emul-boot                   \
				-boot-load-size 4               \
				-A HarOS                           \
				-input-charset utf8             \
				-quiet                          \
				-boot-info-table                \
				-o os.iso                       \
				iso

run: os.iso
	bochs -qf bochsrc

%.o: %.c ${HEADERS}
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf ./*/*.o ./*.dis