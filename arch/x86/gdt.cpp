#include <arch/x86/gdt.hpp>
tss_t tss_entry{};
namespace {
    gdt_ptr_t gdt_ptr{};
    gdt_entry_t gdt_table[GDT_TABLE_SIZE];

    void load_gdt() {
        asm volatile(
        "lgdt (%0)\n"
        "mov $16, %%ax\n"  // 1<<4。低3位为TI和RPL
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        "jmp $0x08, $dummy\n"  // 切换至内核代码段
        "dummy:\n"
        : :"m"(gdt_ptr));
    }
}
namespace x86 {

    void set_gdt_entry(uint8_t index, uint32_t base, uint32_t limit,
                       bool rw, bool executable, uint8_t dpl, bool is_tss) {
        gdt_entry_t *entry = &gdt_table[index];
        entry->base_high = base >> 24 & 0xFF; //高8位
        entry->base_low = base & 0xFFFF; //低16位
        entry->base_middle = (base & (0xFF << 16)) >> 16; //中间8位
        entry->limit_low = (limit & 0xFFFF); //低16位
        entry->limit_middle = (limit >> 16) & 0x0F;
        entry->protected_mode = 1;
        entry->page_granularity = 1;
        entry->dc = 0;
        entry->rw = rw;
        entry->accessed = 0;
        entry->executable = executable;
        entry->dpl = dpl;
        entry->present = 1;
        entry->not_tss = is_tss;
        entry->rsvd = 0;
        if (is_tss) {
            entry->page_granularity = 0;
            entry->accessed = 1;
        }
    }

    void init_gdt() {
        gdt_ptr.limit = (sizeof(gdt_entry_t) * GDT_TABLE_SIZE) - 1;
        gdt_ptr.base = (uint32_t) &gdt_table;
        set_gdt_entry(KERNEL_CS_GDT_INDEX, 0, 0xFFFFF, true, true, 0);  // 内核代码段
        set_gdt_entry(KERNEL_DS_GDT_INDEX, 0, 0xFFFFF, true, false, 0);  // 内核数据段
        set_gdt_entry(USER_CS_GDT_INDEX, 0, 0xFFFFF, true, true, 3);  // 用户代码段
        set_gdt_entry(USER_DS_GDT_INDEX, 0, 0xFFFFF, true, false, 3);  // 用户数据段
        load_gdt();
        init_tss();
    }

    // namespace

    void init_tss() {
        uint32_t base = U32(&tss_entry);
        uint32_t limit = base + sizeof(tss_entry);
        tss_entry.ss0 = KERNEL_DS_SELECTOR;
        tss_entry.esp0 = 0;
        tss_entry.cs = KERNEL_CS_SELECTOR | 0x3;
        tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = KERNEL_DS_SELECTOR | 0x3;
        tss_entry.trap = 0;
        set_gdt_entry(TSS_ENTRY_INDEX, base, limit, false, true, 0, true);
        const uint16_t ltr = TSS_ENTRY_SELECTOR | 0x3;
        asm volatile ("ltr %0\n"::"r"(ltr));
    }
}