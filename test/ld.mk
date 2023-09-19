# ****************************************************************************************************************************************************************************************************************************************************************************
# *  Copyright (c) 2023 SteelManganeseQ356-CPP																																																							   *
# *  QuantumNEC OS is licensed under Mulan PSL v2.																																																						   *
# *  You can use this software according to the terms and conditions of the Mulan PSL v2. 																																												   *
# *  You may obtain a copy of Mulan PSL v2 at:																																																							   *
# *            http://license.coscl.org.cn/MulanPSL2																																																						   *
# *   THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.  														       *
# *   See the Mulan PSL v2 for more details.  																																																							   *
# ****************************************************************************************************************************************************************************************************************************************************************************

# 链接参数
LDFLAGS:= -m elf_x86_64
LDFLAGS+= --allow-multiple-definition 
LDFLAGS+= --no-warn-rwx-segments
LDFLAGS+= -z muldefs

LDFLAGS:=$(strip ${LDFLAGS})		# 合并

# Arch
#BUILD:= ps2.ld
BUILD:= timer.ld
BUILD+= gdt.ld
BUILD+= smp.ld
BUILD+= idt.ld
BUILD+= entry.ld
BUILD+= apic.ld
BUILD+= intr.ld
BUILD+= pic.ld
BUILD+= cxxabi.ld
BUILD+= ArchMain.ld
BUILD+= thread.ld
BUILD+= progress.ld
# Kernel
BUILD+= Kernel.ld
BUILD+= start.ld
BUILD+= Heap.ld
BUILD+= Page.ld
BUILD+= Memory.ld
BUILD+= tss.ld
BUILD+= task.ld
# Lib
BUILD+= Logput.ld
BUILD+= basePrint.ld
BUILD+= list.ld
BUILD+= string.ld
BUILD+= SysIn.ld
BUILD+= SysOut.ld
BUILD+= alloc_table.ld
BUILD+= bitmap.ld
# Utils
BUILD+= bit.ld
BUILD+= io.ld
BUILD+= asm.ld
BUILD+= utils.ld
# Driver
BUILD+= graphics.ld
# Debug
BUILD+= debug.ld
# FS

# Syscall

BUILD:=$(strip ${BUILD})		# 合并

OBJECTFILE:=Kernel.elf
.PHONY: ld
ld:
	
	ld $(LDFLAGS) $(BUILD) -o $(OBJECTFILE)  -T System.lds
	


