cmake_minimum_required(VERSION 3.2)

#Build universal if we are on our m1 mac, also requires using xcode as the build enbine.
if (UNIX AND APPLE)
    set(CMAKE_OSX_ARCHITECTURES arm64;x86_64)
endif(MACOSX)

project(sdl_dash
        VERSION 0.1.0
        LANGUAGES C)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /c/cmake)
include(CPack)

#Add all of our source files and headers to this library.
file(GLOB FILES lua_dash/**/*.c)
file(GLOB MAIN lua_dash/test.c)
list(APPEND FILES ${MAIN})
# Build a win32 bundle when building on win, and macos when building on ios
# Remove macosbundle as its easier to copy the files and debug
#Remove win32 as couldn't build with sdl2
add_executable(supergoon_dash
                # WIN32
                # MACOSX_BUNDLE
                ${FILES})
set_property(TARGET supergoon_dash PROPERTY C_STANDARD 99)
find_package(PkgConfig)
# Generate our EXPORT and NO_EXPORT macros in sgengine_exported.h, this is for automatically generating windows declspecs
#generate_export_header(supergoon_engine EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/supergoon_engine_export.h)
#Search and link to all static libraries, and build them if needed.

find_package(SDL2
2.24.0
NAMES SDL2 sdl2
    PATH_SUFFIXES .a .dll
    REQUIRED)
if(SDL2_FOUND)
    message(STATUS "FOUND SDL2")
else()
    message(STATUS "DID NOT FOUND SDL2, should build and install.")
endif(SDL2_FOUND)

#Check for vorbis and the vorbisfile component.
find_package(VORBIS
    NAMES vorbis Vorbis
    PATH_SUFFIXES .a .dll
    COMPONENTS REQUIRED VORBISFILE VORBIS VORBISENC
    REQUIRED)

if(VORBIS_FOUND)
    message(STATUS "FOUND Vorbis")
    find_library(LIB_VORBIS REQUIRED
    NAMES libvorbis vorbis vorbis.a vorbis.dll libvorbis.a HINTS /usr/local/lib /c/cmake/lib)
    find_library(LIB_VORBISFILE REQUIRED
    NAMES  libvorbisfile vorbisfile vorbisfile.a vorbisfile.dll HINTS /usr/local/lib)
else()
    message(STATUS "DID NOT FOUND VORBIS, should build and install.")
endif(VORBIS_FOUND REQUIRED)

find_package(OGG
NAMES ogg Ogg
PATH_SUFFIXES .a .dll
REQUIRED)
if(OGG_FOUND)
    message(STATUS "FOUND OGG")
else()
    message(STATUS "DID NOT FOUND OGG, should build and install.")
endif(OGG_FOUND)
#Find lua static (non homebrew)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
find_library(LIB_LUA NAMES liblua_static lua_static HINTS /usr/local/lib
REQUIRED)
if(NOT LIB_LUA)
    message(STATUS "DID NOT FOUND LUA static library with find_library, should check if built differently.")
    find_package(Lua)
    if(LUA_FOUND)
        message(STATUS "FOUND LUA")
    else()
        message(STATUS "DID NOT FOUND LUA, should build and install.")
    endif(LUA_FOUND)
else()
    set(LUA_LIBRARIES ${LIB_LUA})
endif(NOT Lua_LIB)





#Debug, print aout all variables.
get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

target_link_libraries(supergoon_dash
    PUBLIC
    PRIVATE ${SDL2_STATIC_LIBRARIES}  ${LUA_LIBRARIES} ${LIB_VORBISFILE} ${LIB_VORBIS} ${OGG_LIBRARIES}
    INTERFACE )

target_include_directories(supergoon_dash
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/"
    "${CMAKE_BINARY_DIR}/exports/"
    ${GAME_INCLUDE_DIRS}
    )

# Put win32 assets indo the Debug place, cause MSVC sucks
if(WIN32)
    set(ASSET_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/assets)
else()
    set(ASSET_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

# Copy folders into the built folder, for assets, scripts, etc for easy distribution.
set(COPY_DIRS config scripts assets)
message(STATUS "The copy dirs are "${COPY_DIRS})
foreach( file_i ${COPY_DIRS})
    add_custom_command(
        TARGET supergoon_dash POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/lua_dash/${file_i}
        ${ASSET_OUTPUT_DIR}/${file_i}
        )
endforeach( file_i )
#Set compiler flags based on system.
if (${CMAKE_C_COMPILER_ID} STREQUAL "GNU")
    message(STATUS "Setting G++ flags")
    target_compile_options(supergoon_dash PRIVATE -std=c99)
elseif(${CMAKE_C_COMPILER_ID} STREQUAL "MSVC")
    message(STATUS "Setting MSVC flags")
    target_compile_options(supergoon_dash PRIVATE /EHsc /W2 /c)
    # Set the DLLEXPORT variable to export symbols
    # target_compile_definitions(supergoon_dash PRIVATE WIN_EXPORT)
elseif(${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    message(STATUS "Setting Clang flags")
    # Activate all clang warnings except those that don't really affect much
    target_compile_options(supergoon_dash PUBLIC -std=c99 -Wall -pedantic -DDEBUG_BUILD_ENABLED)
    target_link_options(supergoon_dash PUBLIC -lm)
elseif(${CMAKE_C_COMPILER_ID} STREQUAL "AppleClang")
    message(STATUS "Setting Apple Clang Flags")
    target_compile_options(supergoon_dash PUBLIC -std=c99 -Wall -pedantic -DDEBUG_BUILD_ENABLED)
else()
    message(STATUS ${CMAKE_C_COMPILER_ID})
    target_compile_options(supergoon_dash PUBLIC /EHsc /W2 /c)
endif()
include(CPack)
