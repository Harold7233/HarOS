C_SOURCES = $(wildcard common/*.cpp *.cpp x86/*.cpp libc/*.cpp memory/*.cpp kernel/*.cpp task/*.cpp devices/*.cpp)
ASM_SOURCES = $(wildcard loader.S x86/*.S )
HEADERS = $(wildcard common/*.hpp x86/*.hpp libc/*.hpp memory/*.hpp memory/*.h task/*.hpp kernel/*.hpp devices/*.hpp)
OBJECTS = ${ASM_SOURCES:.S=.o} ${C_SOURCES:.cpp=.o}

CC = i386-elf-gcc
CFLAGS = -g -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
			-nostartfiles -nodefaultlibs -Wall -Wextra -c -I. 
LDFLAGS = -T link.ld -melf_i386
AS = i386-elf-as -g

all: os.iso
	
kernel.elf: $(OBJECTS)
	i386-elf-ld $(LDFLAGS) $(OBJECTS) -o kernel.elf
	i386-elf-objdump -D kernel.elf > kernel.dis

os.iso: kernel.elf
	cp kernel.elf build/boot
	mkisofs -R                              \
				-b boot/grub/stage2_eltorito    \
				-no-emul-boot                   \
				-boot-load-size 4               \
				-A HarOS                           \
				-input-charset utf8             \
				-quiet                          \
				-boot-info-table                \
				-o os.iso                       \
				build

%.o: %.cpp ${HEADERS}
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf ./*/*.o ./*.dis build/boot/kernel.elf ./kernel.elf