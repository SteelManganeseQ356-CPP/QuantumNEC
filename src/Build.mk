include build.ini
SHELL:= /bin/sh					# 开启source
# 编译所有文件
# %.ld: 
# 	@$(ECHO) Start compiling $*.cpp ...
# 	$(GXX) $(C_CPP_INCLUDE_PATH) $(CPPFLAGS) -c $*.cpp -o $*.ld
# 	@$(ECHO) Done.
# %.sld:
# 	@$(ECHO) Start compiling $*.asm ...
# 	$(NASM) $(NASMFLAGS) $*.asm -o $*.sld
# 	@$(ECHO) Done.
BUILD_FIEL:= \
	$(OBJECT_BUILD_PATH)/obj/kernel.o  \
 	$(OBJECT_BUILD_PATH)/obj/start.o  \
	$(OBJECT_BUILD_PATH)/obj/allocate.o \
	$(OBJECT_BUILD_PATH)/obj/bitmap.o \
	$(OBJECT_BUILD_PATH)/obj/symbol.o \
	$(OBJECT_BUILD_PATH)/obj/base_print.o \
	$(OBJECT_BUILD_PATH)/obj/core.o \
	$(OBJECT_BUILD_PATH)/obj/format.o \
	$(OBJECT_BUILD_PATH)/obj/iostream.o  \
	$(OBJECT_BUILD_PATH)/obj/list.o \
	$(OBJECT_BUILD_PATH)/obj/panic.o \
	$(OBJECT_BUILD_PATH)/obj/assert.o \
	$(OBJECT_BUILD_PATH)/obj/string.o \
	$(OBJECT_BUILD_PATH)/obj/cstring.o \
	$(OBJECT_BUILD_PATH)/obj/cxxabi.o \
	$(OBJECT_BUILD_PATH)/obj/asm.o \
	$(OBJECT_BUILD_PATH)/obj/bit.o \
	$(OBJECT_BUILD_PATH)/obj/utils.o \
	$(OBJECT_BUILD_PATH)/obj/graphics.o \
	$(OBJECT_BUILD_PATH)/obj/cmath.o \
	$(OBJECT_BUILD_PATH)/obj/builtins.o \
	$(OBJECT_BUILD_PATH)/obj/CMOS.o \
	$(OBJECT_BUILD_PATH)/obj/ps2.o \
	$(OBJECT_BUILD_PATH)/obj/timer.o \
	$(OBJECT_BUILD_PATH)/obj/cpu.o \
	$(OBJECT_BUILD_PATH)/obj/smp.o \
	$(OBJECT_BUILD_PATH)/obj/xdt.o \
	$(OBJECT_BUILD_PATH)/obj/entry.o \
	$(OBJECT_BUILD_PATH)/obj/apic.o \
	$(OBJECT_BUILD_PATH)/obj/interrupt.o \
	$(OBJECT_BUILD_PATH)/obj/pic_8259A.o \
	$(OBJECT_BUILD_PATH)/obj/arch-driver.o \
	$(OBJECT_BUILD_PATH)/obj/memory.o \
	$(OBJECT_BUILD_PATH)/obj/stack.o \
	$(OBJECT_BUILD_PATH)/obj/paging.o \
	$(OBJECT_BUILD_PATH)/obj/heap.o \
	$(OBJECT_BUILD_PATH)/obj/map.o \
	$(OBJECT_BUILD_PATH)/obj/platform.o

%.elf: $(BUILD_FIEL)
	@$(ECHO) Start linking all obj-file ...
	$(LD) $(LDFLAGS) $^ -o $*.elf  -T $(OBJECT_BUILD_PATH)/obj/System.lds
	@$(ECHO) Done.
%.efi:
	@$(ECHO) Start building UEFI boot file ...
	source "$(THE_FILE_ULD_PATH)/build.sh"
	@$(ECHO) Done.
.PHONY: buildDir
buildDir:
	mkdir "$(OBJECT_BUILD_PATH)"
	mkdir "$(OBJECT_BUILD_PATH)/obj"
	mkdir "$(OBJECT_BUILD_PATH)/esp"
	mkdir "$(OBJECT_BUILD_PATH)/esp/EFI"
	mkdir "$(OBJECT_BUILD_PATH)/esp/EFI/Boot"
	mkdir "$(OBJECT_BUILD_PATH)/esp/QuantumNEC"
	mkdir "$(OBJECT_BUILD_PATH)/esp/QuantumNEC/font"
	mkdir "$(OBJECT_BUILD_PATH)/esp/QuantumNEC/Logger"
	cp $(OVME) $(OBJECT_BUILD_PATH)/ -r
	cp $(THE_FILE_PATH)/Boot/Info/target.txt $(EDK2_PATH)/Conf/ -r
	cp $(THE_FILE_PATH)/build.sh $(THE_FILE_ULD_PATH)/ -r
	cp $(THE_FILE_PATH) $(EDK2_PATH)/QuantumNECPkg -r
	cp $(THE_FILE_PATH)/Boot/Logo/SystemLogo.bmp $(OBJECT_BUILD_PATH)/esp/EFI/Boot/Logo.BMP -r
	cp "$(THE_FILE_PATH)/Boot/Logo/SystemLogo(4-3).bmp" $(OBJECT_BUILD_PATH)/esp/EFI/Boot/Narrow.BMP -r
	cp $(THE_FILE_PATH)/Boot/Info/SystemLog.log $(OBJECT_BUILD_PATH)/esp/QuantumNEC/Logger/SystemLogger.log
	cp $(THE_FILE_ULD_PATH)/font/Unicode.dll $(OBJECT_BUILD_PATH)/esp/QuantumNEC/font/ -r
	cp $(THE_FILE_PATH)/Boot/Info/Config.ini $(OBJECT_BUILD_PATH)/esp/EFI/Boot/Config.ini -r
	cp $(THE_FILE_PATH)/Arch/$(OBJ_ARCH)/System.lds $(OBJECT_BUILD_PATH)/obj/System.lds -r
.PHONY: outFile
outFile:
	@echo Start building *.cpp file...
	@$(MAKE) -C $(THE_FILE_PATH)/Kernel/
	@$(MAKE) -C $(THE_FILE_PATH)/Lib/Base/
	@$(MAKE) -C $(THE_FILE_PATH)/Lib/Debug/
	@$(MAKE) -C $(THE_FILE_PATH)/Lib/IO/
	@$(MAKE) -C $(THE_FILE_PATH)/Lib/STL/
	@$(MAKE) -C $(THE_FILE_PATH)/Lib/SystemCall/
	@$(MAKE) -C $(THE_FILE_PATH)/Arch/
	@$(MAKE) -C $(THE_FILE_PATH)/Utils/
	@$(MAKE) -C $(THE_FILE_PATH)/Driver/
	@echo Done.
.PHONY: Boot
Boot: $(EFI_NAME)
	cp $(EDK2_PATH)/Build/DEBUG_GCC5/X64/$(EFI_NAME) $(OBJECT_BUILD_PATH)/esp/EFI/Boot/ -r
	@$(ECHO) Done.
.PHONY: Kernel
Kernel: \
	outFile \
	$(KERNEL_OUTPUTL_FILE_NAME)
.PHONY: clean
clean:
	$(RM) -rf $(OBJECT_BUILD_PATH)
	$(RM) -rf $(EDK2_PATH)/QuantumNECPkg
	$(RM) -rf ../build.sh
	$(CLS)
.PHONY: run
run:
	$(QEMU) $(QEMUFLAGS)