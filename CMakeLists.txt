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

# One place for platform/system libs needed by Sokol backends
add_library(platform_deps INTERFACE)
if(WIN32)
    # GL backend
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
	# Produce .html per executable (otherwise you'll get .js)
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
	# Tell Sokol to use WebGL2 (GLES3)
	target_compile_definitions(platform_deps INTERFACE SOKOL_GLES3)
	# Emscripten link options (tweak as needed)
	target_link_options(platform_deps INTERFACE
	"-sUSE_WEBGL2=1"
	"-sFULL_ES3=1"
	"-sWASM=1"
	"-sALLOW_MEMORY_GROWTH=1"
	"-sASSERTIONS=1"
	"-sSINGLE_FILE=1"          # Embed all JS and WASM into a single file
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
	add_compile_options(/MP)   # or /MP8 to cap the number of cl.exe processes
endif()

# Engine static lib
add_subdirectory(engine)

# Samples
add_subdirectory(samples/snake)

# Visual Studio startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Snake)