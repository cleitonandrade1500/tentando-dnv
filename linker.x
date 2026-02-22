ENTRY(_start)
PHDRS { text PT_LOAD FLAGS(5); data PT_LOAD FLAGS(6); }
SECTIONS {
  .text : { *(.text._start) *(.text*) } : text
  .data : { *(.data*) *(.rodata*) } : data
  .bss : { *(.bss*) } : data
}
