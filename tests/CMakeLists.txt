# tests/CMakeLists.txt
add_executable(Tests
    test_scene.cpp
	test_canvas.cpp
)

# Link against the engine library, Catch2, and platform dependencies
target_link_libraries(Tests PRIVATE
    Engine
    Catch2::Catch2WithMain
    SokolStub
)

# Include directories for engine and Catch2
target_include_directories(Tests PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_BINARY_DIR}/engine/generated
    ${sokol_SOURCE_DIR}
)

if(WIN32)
    set_target_properties(Tests PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()

# Ensure tests are built for Emscripten correctly
if(EMSCRIPTEN)
    set_target_properties(Tests PROPERTIES
        SUFFIX ".html"
    )
endif()

#set_target_properties(Tests PROPERTIES
#    VS_DEBUGGER_COMMAND_ARGUMENTS "-o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/results.txt"
#)

include(Catch)
catch_discover_tests(Tests)

#if(WIN32)
#    add_custom_command(TARGET Tests POST_BUILD
#        COMMAND type "\"$<SHELL_PATH:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/results.txt>\""
#    )
#elseif(APPLE)
#    add_custom_command(TARGET Tests POST_BUILD
#        COMMAND cat "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/results.txt"
#    )
#endif()