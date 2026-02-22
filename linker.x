ENTRY(module_start)
PHDRS {
  text PT_LOAD FLAGS(5);
  data PT_LOAD FLAGS(6);
}
SECTIONS {
  . = 0x1000000; /* Área de memória segura para não dar crash no Havana */
  .text : { *(.text*) } : text
  .data : { *(.data*) *(.rodata*) } : data
  .bss : { *(.bss*) } : data
}
