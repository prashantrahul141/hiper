# Real mode guest examples
REAL_BUILD_DIR = $(PWD)/../build/guest/real
REAL_SRC_DIR = $(PWD)/real/
REAL_LINK_SCRIPT = $(REAL_SRC_DIR)/guest_real_link.ld

# hello world
REAL_BUILD_DIR_HELLOWORLD = $(REAL_BUILD_DIR)/helloworld
REAL_SRC_DIR_HELLOWORLD = $(REAL_SRC_DIR)/helloworld
$(REAL_BUILD_DIR_HELLOWORLD)/helloworld: $(REAL_BUILD_DIR_HELLOWORLD)/helloworld.o $(REAL_LINK_SCRIPT)
	ld -m elf_i386 -T $(REAL_LINK_SCRIPT) --oformat binary $< -o $@

$(REAL_BUILD_DIR_HELLOWORLD)/helloworld.o: $(REAL_SRC_DIR_HELLOWORLD)/helloworld.S $(REAL_BUILD_DIR)
	as --32 $< -o $@

$(REAL_BUILD_DIR):
	mkdir -p $@/helloworld


# Protected mode guest examples
PROT_BUILD_DIR = $(PWD)/../build/guest/protected
PROT_SRC_DIR = $(PWD)/protected
PROT_LINK_SCRIPT = $(PROT_SRC_DIR)/guest.ld
