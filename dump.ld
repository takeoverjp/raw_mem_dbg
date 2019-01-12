SECTIONS
{
  . = 0x00400390,
  .text : { *(.text) }
  . = 0x006cb080,
  .data : { *(.data) }
}
