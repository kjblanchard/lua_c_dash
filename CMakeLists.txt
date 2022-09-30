cmake_minimum_required(VERSION 3.2)
project(sdl_dash
        VERSION 0.1.0
        LANGUAGES C)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
include(CPack)
# Build OSX universal
set(CMAKE_OSX_ARCHITECTURES arm64)
link_directories(/usr/local/lib)





#Add all of our source files and headers to this library.
# file(GLOB FILES supergoon_dash/**/*.?pp)
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

pkg_check_modules(GAME lua openal vorbisfile vorbis ogg sdl2 REQUIRED lua openal vorbisfile vorbis ogg sdl2) #Used this when not building from source

message(STATUS "Files are ${GAME_STATIC_LIBRARIES}")

target_link_libraries(supergoon_dash
    PUBLIC  ${GAME_STATIC_LIBRARIES}
                        PRIVATE 
                        INTERFACE )

message(STATUS "Copying the Libraries to the output folder.  We do this for FMOD since we aren't building from source.")
#foreach( file_i ${COPY_LIBS})
#    add_custom_command(
#    TARGET supergoon_engine
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND}
#    ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/bin/${file_i} ${EXTERNAL_LIBRARY_OUTPUT_DIR}/${file_i}
#    )
#endforeach( file_i )

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
    set(ASSET_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)
endif()

# Copy assets into the built folder.
#add_custom_command(
#        TARGET supergoon_dash POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#                ${CMAKE_CURRENT_SOURCE_DIR}/assets
#                ${ASSET_OUTPUT_DIR}
#        )

if (${CMAKE_C_COMPILER_ID} STREQUAL "GNU")
    message(STATUS "Setting G++ flags")
    target_compile_options(supergoon_dash PRIVATE -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-newline-eof -Wno-padded -Wno-exit-time-destructors -Wno-global-constructors -Wno-constant-conversion)
elseif(${CMAKE_C_COMPILER_ID} STREQUAL "MSVC")
    message(STATUS "Setting MSVC flags")
    target_compile_options(supergoon_dash PRIVATE /EHsc /W2 /c)
    # Set the DLLEXPORT variable to export symbols
    # target_compile_definitions(supergoon_dash PRIVATE WIN_EXPORT)
elseif(${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    message(STATUS "Setting Clang flags")
    # Activate all clang warnings except those that don't really affect much
    target_compile_options(supergoon_dash PRIVATE -std=c99 -Wall -Werror -pedantic -DDEBUG_BUILD_ENABLED)
elseif(${CMAKE_C_COMPILER_ID} STREQUAL "AppleClang")
    message(STATUS "Setting Apple Clang Flags")
    target_compile_options(supergoon_dash PRIVATE -std=c99 -Wall -Werror -pedantic -DDEBUG_BUILD_ENABLED)
else()
    message(STATUS ${CMAKE_C_COMPILER_ID})
    target_compile_options(supergoon_dash PRIVATE /EHsc /W2 /c)

endif()
# Set relative link for this lib.
set_target_properties(supergoon_dash
    PROPERTIES LINK_FLAGS "-Wl,-rpath,../lib")
