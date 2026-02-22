ENTRY(_main)
PHDRS {
  text PT_LOAD FLAGS(5);
  data PT_LOAD FLAGS(6);
}
SECTIONS {
  . = 0x400000;
  .text : {
    FILL(0x90909090);
    *(.text*)
  } : text
  
  .data : { *(.data*) *(.rodata*) } : data
  .bss : { *(.bss*) } : data

  /DISCARD/ : { *(.comment) *(.note*) }
}
