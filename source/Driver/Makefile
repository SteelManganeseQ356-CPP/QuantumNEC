include ../build.ini
.PHONY: build
build:
	$(GXX) $(C_CPP_INCLUDE_PATH) $(CPPFLAGS) -c ./Display/graphics.cpp -o $(OBJECT_BUILD_PATH)/obj/graphics.o
	$(GXX) $(C_CPP_INCLUDE_PATH) $(CPPFLAGS) -c ./Acpi/acpi.cpp 	   -o $(OBJECT_BUILD_PATH)/obj/acpi.o
	$(GXX) $(C_CPP_INCLUDE_PATH) $(CPPFLAGS) -c ./Audio/audio.cpp 	   -o $(OBJECT_BUILD_PATH)/obj/audio.o
	$(GXX) $(C_CPP_INCLUDE_PATH) $(CPPFLAGS) -c ./driver.cpp 	   	   -o $(OBJECT_BUILD_PATH)/obj/driver.o