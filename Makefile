#Meke it so that dir and build will always be performed.
.PHONY: dir build
#Build the following targets by default when you type make
all: build

CC = clang
BUILD_FOLDER = build
BINARY_NAME = game
FILES = ./lua_dash/*.c ./lua_dash/core/*.c
SCRIPTS = ./lua_dash/*.lua

dir:
	mkdir -p $(BUILD_FOLDER)
build: dir
	$(CC) $(FILES) `sdl2-config --cflags --libs` `pkg-config --libs SDL2_mixer` `pkg-config --static --libs lua` -o $(BUILD_FOLDER)/$(BINARY_NAME)
	cp $(SCRIPTS) $(BUILD_FOLDER)

run:
	./$(BUILD_FOLDER)/$(BINARY_NAME)

debug:
	$(CC) $(FILES) `sdl2-config --cflags --libs` `pkg-config --libs SDL2_mixer` `pkg-config --static --libs lua` -o $(BUILD_FOLDER)/$(BINARY_NAME) -g
	cp $(SCRIPTS) $(BUILD_FOLDER)
static:
	clang test.c `sdl2-config --cflags --static-libs` `pkg-config --static --libs SDL2_mixer` `pkg-config --static --libs lua`

clean:
	rm -rf build


