include ../build.ini
.PHONY: build
build:
	@$(MAKE) -C ./$(OBJ_ARCH)/abi/
	@$(MAKE) -C ./$(OBJ_ARCH)/cpu/
	@$(MAKE) -C ./$(OBJ_ARCH)/driver/
	@$(MAKE) -C ./$(OBJ_ARCH)/interrupt/
	@$(MAKE) -C ./$(OBJ_ARCH)/platform/