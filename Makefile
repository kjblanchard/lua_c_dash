#Meke it so that dir and build will always be performed.
#Build the following targets by default when you type make
.PHONY: debug rebuild
BUILD_FOLDER = build

all: build

CC = clang
BINARY_NAME = $(BUILD_FOLDER)/game
SRC_FOLDERS = ./lua_dash/core \
			  ./lua_dash/sound \
			  ./lua_dash
# the := means that it is only assigned once, and then read after.
#SRC_FILES := $(shell find $(SRC_FOLDERS) -name 'hello.c')
SRC_FILES := $(shell find $(SRC_FOLDERS) -maxdepth 1 -name '*.c')
#how to debug.  Also can use error to stop execution
#$(info $(SRC_FILES))

# Generate a list of .o files from the .c files. Prefix them with the build
# folder to output the files there
OBJ_FILES = $(addprefix $(BUILD_FOLDER)/,$(SRC_FILES:.c=.o))
# Generate a list of depfiles, used to track includes. The file name is the same
# as the object files with the .d extension added
DEP_FILES = $(addsuffix .d,$(OBJ_FILES))
# Flags to generate the .d dependency-tracking files when we compile.  It's
# named the same as the target file with the .d extension
DEPFLAGS = -MMD -MP -MF $@.d
# Include the dependency tracking files
-include $(DEP_FILES)
SCRIPTS = ./lua_dash/*.lua
LDFLAGS = `sdl2-config --cflags --libs` `pkg-config --libs openal` `pkg-config --libs sndfile` `pkg-config --libs lua`
CFLAGS = -std=c99


# The rule for compiling the SRC_FILES into OBJ_FILES $< means first prerequisite
$(BUILD_FOLDER)/%.o: %.c
	echo Compiling $(notdir $<)
	@# Create the folder structure for the output file
	@mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# OBJ_FILES are prerequisites. Since we're not relying on an implicit rule, we need to
# explicity list CFLAGS, LDFLAGS, LDLIBS
# $^ means all prerequisites
build: $(OBJ_FILES)
	echo Linking $(notdir $@)
	@$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $(BINARY_NAME)
	@cp $(SCRIPTS) $(BUILD_FOLDER)

rebuild: $(OBJ_FILES)
	echo Linking $(notdir $@)
	@$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $(BINARY_NAME)
	@cp $(SCRIPTS) $(BUILD_FOLDER)

run:
	@./$(BUILD_FOLDER)/$(BINARY_NAME)

debug: CFLAGS += -g
debug: clean | rebuild

clean:
	@ - rm -rf build


