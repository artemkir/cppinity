cmake_minimum_required(VERSION 3.20)
project(Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    sokol
    GIT_REPOSITORY https://github.com/floooh/sokol
    GIT_TAG master  # Consider using a specific tag for stability
)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG v3.8.1  # Use a stable version
)
FetchContent_MakeAvailable(sokol catch2)

# Set output directory for all configurations
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)

# One place for platform/system libs needed by Sokol backends
add_library(platform_deps INTERFACE)
if(WIN32)
    find_package(OpenGL REQUIRED)
    target_link_libraries(platform_deps INTERFACE
        OpenGL::GL
        kernel32 user32 gdi32 winmm imm32 ole32 oleaut32 version uuid advapi32 setupapi shell32
    )
elseif(APPLE)
    find_package(OpenGL REQUIRED)
    target_link_libraries(platform_deps INTERFACE
        OpenGL::GL
        "-framework Cocoa" "-framework QuartzCore"
    )
elseif(EMSCRIPTEN)
    message(STATUS "Building for WebAssembly (Emscripten)")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    target_compile_definitions(platform_deps INTERFACE SOKOL_GLES3)
    target_link_options(platform_deps INTERFACE
        "-sUSE_WEBGL2=1"
        "-sFULL_ES3=1"
        "-sWASM=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sASSERTIONS=1"
        "-sSINGLE_FILE=1"
    )
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_options(platform_deps INTERFACE
            "-gsource-map"
            "-sASSERTIONS=2"
            "-sGL_DEBUG=1"
        )
    endif()
elseif(UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
    target_link_libraries(platform_deps INTERFACE
        OpenGL::GL
        ${X11_LIBRARIES}
        dl m
    )
endif()

if(MSVC)
    add_compile_options(/MP)
endif()

# Enable testing
enable_testing()

# Engine static lib
add_subdirectory(engine)

# Samples
add_subdirectory(samples/snake)

# Tests
add_subdirectory(tests)

# Visual Studio startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Snake)