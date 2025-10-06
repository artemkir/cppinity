cmake_minimum_required(VERSION 3.20)
project(Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    sokol
    GIT_REPOSITORY https://github.com/floooh/sokol
    GIT_TAG master  # Use a specific commit or tag for stability, e.g., 'v1.0.0' if available
)
FetchContent_MakeAvailable(sokol)

# Set output directory for all configurations
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)

# Collect all .cpp and .h files recursively
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.c)
file(GLOB_RECURSE GAME_HEADERS CONFIGURE_DEPENDS include/*.h)

add_executable(Game ${GAME_SOURCES} ${GAME_HEADERS})
target_include_directories(Game PRIVATE ${sokol_SOURCE_DIR} include)  # Add 'include' for project headers

# Platform-specific linking for Sokol backends (adjust based on your SOKOL_* define)
if(WIN32)
    # For GL backend
    find_package(OpenGL REQUIRED)
    target_link_libraries(Game PRIVATE OpenGL::GL kernel32 user32 gdi32 winmm imm32 ole32 oleaut32 version uuid advapi32 setupapi shell32)
elseif(UNIX AND NOT APPLE)
    # For Linux, link GL and X11
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
    target_link_libraries(Game PRIVATE OpenGL::GL ${X11_LIBRARIES} dl m)
elseif(APPLE)
    # For macOS, link OpenGL and Cocoa (Sokol uses Metal or GL)
    find_package(OpenGL REQUIRED)
    target_link_libraries(Game PRIVATE OpenGL::GL "-framework Cocoa" "-framework QuartzCore")
endif()

# Set Game as the startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Game)
 
# Set debugger properties for VS
set_target_properties(Game PROPERTIES
    VS_DEBUGGER_COMMAND "$<TARGET_FILE:Game>"
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Game>"
)