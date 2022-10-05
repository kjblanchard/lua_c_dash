cmake_minimum_required(VERSION 3.2)

#Build universal if we are on our m1 mac, also requires using xcode as the build engine when doing this.  -G Xcode.
# Must be called before project.
if (UNIX AND APPLE)
    set(CMAKE_OSX_ARCHITECTURES arm64;x86_64)
endif(UNIX AND APPLE)

project(sdl_dash
    VERSION 0.1.0
    LANGUAGES C)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
#Add in the windows cmake path to prefix, so that we can find things.
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /c/cmake)
set(CMAKE_OSX_DEPLOYMENT_TARGET 12.3)
include(CPack)

#Add all of our source files
file(GLOB FILES lua_dash/**/*.c)
file(GLOB MAIN lua_dash/*.c)
list(APPEND FILES ${MAIN})
# Build a win32 bundle when building on win, and macos when building on ios,  Remove macosbundle as its easier to copy the files and debug #Remove win32 as couldn't build with sdl2
add_executable(supergoon_dash
    # WIN32
    # MACOSX_BUNDLE
    ${FILES})
set_property(TARGET supergoon_dash PROPERTY C_STANDARD 99)
find_package(PkgConfig)

#Find all required packages, these should be built prior, they are all linked statically.
find_package(SDL2
    2.24.0
    NAMES SDL2 sdl2
    PATH_SUFFIXES .a
    REQUIRED)

#Potentially needed for windows / unix.
#find_package(VORBIS
#    NAMES vorbis Vorbis
#    PATH_SUFFIXES .a
#    COMPONENTS REQUIRED VORBISFILE VORBIS VORBISENC
#    REQUIRED)

find_library(LIB_VORBIS
    REQUIRED
    PATH_SUFFIXES .a
    NAMES libvorbis vorbis vorbis.a vorbis.dll libvorbis.a
    HINTS /usr/local/lib /c/cmake/lib)

find_library(LIB_VORBISFILE
    REQUIRED
    NAMES  libvorbisfile vorbisfile vorbisfile.a vorbisfile.dll
    HINTS /usr/local/lib)

find_package(
    OGG
    NAMES ogg Ogg
    PATH_SUFFIXES .a
    REQUIRED)

#Find lua static (non homebrew)
find_library(LIB_LUA
    NAMES liblua_static lua_static  liblua_static.a lua_static.a
    HINTS /usr/local/lib
    REQUIRED)
set(LUA_LIBRARIES ${LIB_LUA})

#For Debug, print aout all variables.
#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

#Order matters
target_link_libraries(supergoon_dash
    PUBLIC
    PRIVATE ${SDL2_STATIC_LIBRARIES}  ${LUA_LIBRARIES} ${LIB_VORBISFILE} ${LIB_VORBIS} ${OGG_LIBRARIES}
    INTERFACE )

target_include_directories(supergoon_dash
    PUBLIC
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/"
    )

# Copy folders into the built folder, for assets, scripts, etc for easy distribution.
set(COPY_DIRS config scripts assets)
foreach( file_i ${COPY_DIRS})
    add_custom_command(
        TARGET supergoon_dash POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/lua_dash/${file_i}
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${file_i}
        )
endforeach( file_i )

if(${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    message(STATUS "Setting Clang flags")
    target_compile_options(supergoon_dash PUBLIC -std=c99 -Wall -Werror -pedantic -DDEBUG_BUILD_ENABLED)
    target_link_options(supergoon_dash PUBLIC -lm)
elseif(${CMAKE_C_COMPILER_ID} STREQUAL "AppleClang")
    message(STATUS "Setting Apple Clang Flags")
    target_compile_options(supergoon_dash PUBLIC -std=c99 -Wall -Werror -pedantic -DDEBUG_BUILD_ENABLED)
else()
    message(STATUS ${CMAKE_C_COMPILER_ID})
    target_compile_options(supergoon_dash PUBLIC /EHsc /W2 /c)
endif()
