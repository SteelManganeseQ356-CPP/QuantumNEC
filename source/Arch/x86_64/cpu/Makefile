include ../../../build.ini
.PHONY: build
build:
	$(GXX) $(C_CPP_INCLUDE_PATH) $(CPPFLAGS) -c ./cpu.cpp 		-o	$(OBJECT_BUILD_PATH)/obj/cpu.o
	$(GXX) $(C_CPP_INCLUDE_PATH) $(CPPFLAGS) -c ./smp.cpp 		-o 	$(OBJECT_BUILD_PATH)/obj/smp.o
	$(GXX) $(C_CPP_INCLUDE_PATH) $(CPPFLAGS) -c ./xdt.cpp 		-o 	$(OBJECT_BUILD_PATH)/obj/xdt.o
	$(NASM) $(ASMFLAGS)                         ./entry.asm 	-o 	$(OBJECT_BUILD_PATH)/obj/entry.o