OUTPUT_ARCH( "riscv" )
ENTRY( _start )

SECTIONS
{
  /* Ξεκινάμε στην 0x80000000 */
  . = 0x80000000;

  .text : {
    *(.text.init)
    *(.text .text.*)
  }

  /* Ευθυγράμμιση σε 4KB για να μην έχουμε overlap */
  . = ALIGN(0x1000);

  .rodata : { *(.rodata .rodata.*) }
  .data : { *(.data .data.*) }
  
  .bss : {
    *(.bss .bss.*)
  }

  . = ALIGN(16);
  stack_start = .;
  . = . + 0x4000; 
  stack_top = .;
}