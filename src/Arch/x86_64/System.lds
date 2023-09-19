/* x86_64的lds文件 */
OUTPUT_FORMAT("elf64-x86-64","elf64-x86-64","elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
ENTRY(microKernelEntry)
SECTIONS
{

    . = 0xffff800000000000 + 0x100000;
	.text :
	{
		_text = .;
		*(.text)
		_etext = .;
	}
	. = ALIGN(8);
	.data :
	{
		_data = .;
		*(.data)		
		_edata = .;
	}
	.rodata : 
	{
		_rodata = .;	
		*(.rodata)
		_erodata = .;
	}
	. = ALIGN(8);
	.KSymbol_table :
	{
		_kernel_symbol_begin = .;
		*(.KSymbol_table)
		_kernel_symbol_end = .;
	}
	. = ALIGN(8);
	.KString_table :
	{
		*(.KString_table)
	}
	. = ALIGN(32768);
	.data.init_task : { *(.data.init_task) }
	.bss :
	{
		_bss = .;
		*(.bss)
		*(COMMON)
		_ebss = .;
	}
	_end = .;
	_debug = .;
    .debug_aranges :{*(.debug_aranges)}
    .debug_abbrev :{*(.debug_abbrev)}
    .debug_line :{*(.debug_line)}
    .debug_frame :{*(.debug_frame)}
    .debug_str :{*(.debug_str)}
    _edebug = .; 
}