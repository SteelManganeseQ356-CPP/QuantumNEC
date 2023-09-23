include ../../../build.ini
.PHONY: build
build:
	$(GXX) $(C_CPP_INCLUDE_PATH) $(CPPFLAGS) -c ./apic.cpp 			-o $(OBJECT_BUILD_PATH)/obj/apic.o
	$(GXX) $(C_CPP_INCLUDE_PATH) $(CPPFLAGS) -c ./interrupt.cpp 	-o $(OBJECT_BUILD_PATH)/obj/interrupt.o
	$(GXX) $(C_CPP_INCLUDE_PATH) $(CPPFLAGS) -c ./pic_8259A.cpp		-o $(OBJECT_BUILD_PATH)/obj/pic_8259A.o