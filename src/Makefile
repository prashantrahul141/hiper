BUILD_DIR = $(PWD)/../build/hiper
SRC_DIR = $(PWD)

SAN_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer
HARDEN_FLAGS := -fstack-protector-strong -D_FORTIFY_SOURCE=2 -fPIE

MODE ?= debug

ifeq ($(MODE),debug)

CFLAGS := -std=c17 -Wall -Wextra -O0 -g3 \
          $(SAN_FLAGS)
LDFLAGS := $(SAN_FLAGS)

else ifeq ($(MODE),release)

CFLAGS := -std=c17 -Wall -Wextra -O2 \
      $(HARDEN_FLAGS)
LDFLAGS := -pie

else
$(error Unknown MODE, run with `Make MODE=[release|debug]`: $(MODE))
endif

C_INCLUDES := -I$(SRC_DIR)

C_SOURCES := $(SRC_DIR)/nhlog.c \
			 $(SRC_DIR)/opt.c \
			 $(SRC_DIR)/common.c  \
			 $(SRC_DIR)/kvm.c  \
			 $(SRC_DIR)/vcpu.c  \
             $(SRC_DIR)/main.c

C_OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))

$(BUILD_DIR)/hiper: $(C_OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(C_INCLUDES) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@
