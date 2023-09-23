include ../build.ini
.PHONY: build
build:
	$(GXX) $(C_CPP_INCLUDE_PATH) $(CPPFLAGS) -c start.cpp 									-o $(OBJECT_BUILD_PATH)/obj/start.o
	$(GXX) $(C_CPP_INCLUDE_PATH) $(CPPFLAGS) -c kernel.cpp 									-o $(OBJECT_BUILD_PATH)/obj/kernel.o
	$(GXX) $(C_CPP_INCLUDE_PATH) $(CPPFLAGS) -c memory.cpp									-o $(OBJECT_BUILD_PATH)/obj/memory.o
	$(GXX) $(C_CPP_INCLUDE_PATH) $(CPPFLAGS) -c $(THE_FILE_PATH)/Kernel/Memory/paging.cpp	-o $(OBJECT_BUILD_PATH)/obj/paging.o
	$(GXX) $(C_CPP_INCLUDE_PATH) $(CPPFLAGS) -c $(THE_FILE_PATH)/Kernel/Memory/map.cpp		-o $(OBJECT_BUILD_PATH)/obj/map.o
	$(GXX) $(C_CPP_INCLUDE_PATH) $(CPPFLAGS) -c $(THE_FILE_PATH)/Kernel/Memory/stack.cpp	-o $(OBJECT_BUILD_PATH)/obj/stack.o
	$(GXX) $(C_CPP_INCLUDE_PATH) $(CPPFLAGS) -c $(THE_FILE_PATH)/Kernel/Memory/heap.cpp		-o $(OBJECT_BUILD_PATH)/obj/heap.o