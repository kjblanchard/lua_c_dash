.PHONY: config configure build release clean rebuild run lldb debug doc windows

BUILD_FOLDER = build/bin
BINARY_NAME = supergoon_dash
BINARY_PATH = $(BUILD_FOLDER)/$(BINARY_NAME)

all: build

configure:
	@cmake . -B build -D CMAKE_BUILD_TYPE=Debug

windows:
	@cmake . -B build -DCMAKE_PREFIX_PATH=/c/cmake/ -G'MinGW Makefiles'

release:
	@cmake . -B build -G Xcode  && cmake --build build --config Release

build:
	@cmake --build build

clean:
	@ - rm -rf build

rebuild: clean configure build install

install:
	@cmake --install build

package:
	cd ./build && tar -czf game.tar.gz bin

run:
	@cd ./$(BUILD_FOLDER) && ./$(BINARY_NAME)

lldb:
	@cd ./$(BUILD_FOLDER) && lldb ./$(BINARY_NAME)

debug: rebuild lldb

doc:
	@cd ./doc/; ./update_doc.sh; cd -

