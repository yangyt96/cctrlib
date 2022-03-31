TARGET_EXEC:=main

# directory
BUILD_DIR:=./build
SRC_DIR:=./cctrlib
TEST_DIR:=./test
INCLUDE_DIRS:= ./ ./include/

# files
SRCS:=$(shell find $(SRC_DIR) -type f -iname '*.c')
TESTS:=$(shell find $(TEST_DIR) -type f -iname '*.c')
OBJS:=$(SRCS:%=$(BUILD_DIR)/%.o) $(TESTS:%=$(BUILD_DIR)/%.o)

# compiler
CC:=$(CROSS_COMPILE)gcc
C_FLAGS:=-Wall -std=gnu17
C_INCLUDES:=$(INCLUDE_DIRS:%=-I %)


all: $(TARGET_EXEC)

$(TARGET_EXEC): $(OBJS)
	$(CC) $(C_FLAGS) $(OBJS) -o $@ $(C_INCLUDES)

$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< -o $@ $(C_INCLUDES)

.PHONY: clean
clean:
	@rm -f $(TARGET_EXEC)
	@rm -rf $(BUILD_DIR)

