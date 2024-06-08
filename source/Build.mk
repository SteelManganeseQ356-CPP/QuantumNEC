%.efi:
	source "./build.sh"
.PHONY: Boot
Boot: BootX64.efi
