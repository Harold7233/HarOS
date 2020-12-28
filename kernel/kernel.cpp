#include <sys/asm.hpp>
#include <devices/vga.hpp>
#include <arch/x86/gdt.hpp>
#include <arch/x86/idt.hpp>
#include <arch/x86/timer.hpp>
#include <kernel/stdio.hpp>
#include <kernel/memory/MemoryManager.hpp>
#include <kernel/tasking/Scheduler.hpp>
#include <kernel/fork.hpp>
#include <boot/multiboot.h>
#include <devices/FloppyDisk.hpp>
#include <devices/RamDisk.hpp>
#include <sys/gcc.hpp>
#include <devices/PCI/PCIManager.hpp>
#include <arch/x86/asm.hpp>

namespace kernel {

    void kmain(multiboot_info_t *mboot_ptr) {
        x86::init_gdt();
        x86::init_idt();
        MemoryManager::init(mboot_ptr);
        devices::vga::init();

        Scheduler::init();
        PCIManager::init();
        x86::Timer::init();
        enable_irq();
        loop();
    }
}

