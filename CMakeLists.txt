cmake_minimum_required(VERSION 3.20)
project(Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.0
)
FetchContent_MakeAvailable(SDL2)

# Set output directory for all configurations
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)

# Collect all .cpp and .h files recursively
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE GAME_HEADERS CONFIGURE_DEPENDS include/*.h)

add_executable(Game ${GAME_SOURCES} ${GAME_HEADERS})
target_include_directories(Game PRIVATE ${sdl2_SOURCE_DIR}/include include)
target_link_libraries(Game PRIVATE SDL2 SDL2main)

# Copy SDL2.dll to output directory
if (WIN32)
    add_custom_command(TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2> $<TARGET_FILE_DIR:Game>
    )
endif()

# Set Game as the startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Game)
 
# Set debugger properties for VS
set_target_properties(Game PROPERTIES
    VS_DEBUGGER_COMMAND "$<TARGET_FILE:Game>"
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Game>"
)