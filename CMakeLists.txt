cmake_minimum_required(VERSION 3.20)
project(Gaianox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Path to SFML cmake config
set(SFML_DIR "$ENV{SFML_ROOT}/lib/cmake/SFML")

find_package(SFML 2.6 COMPONENTS graphics window audio system REQUIRED)

# Add executable with your source files
add_executable(Gaianox
        src/main.cpp
        src/Utilities.h
        src/Animation.h
        src/TextureManager.h
        src/Block.h
        src/Chunk.h
        src/RenderedChunks.h
        src/Game.h
        src/MainMenu.h
        src/PerlinNoise.h
        src/Steve.h
        src/Game.h
        # Add other source files here if any, e.g. src/Utilities.cpp
)

# Include your headers directory
target_include_directories(Gaianox PRIVATE src)

# Include SFML headers (optional if find_package already does it)
target_include_directories(Gaianox PRIVATE "$ENV{SFML_ROOT}/include")

# Link SFML libraries
target_link_libraries(Gaianox PRIVATE
        sfml-graphics
        sfml-window
        sfml-audio
        sfml-system
)

if(WIN32)
    # Release DLLs
    add_custom_command(TARGET Gaianox POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$ENV{SFML_ROOT}/bin/sfml-graphics-2.dll"
            $<TARGET_FILE_DIR:Gaianox>)
    add_custom_command(TARGET Gaianox POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$ENV{SFML_ROOT}/bin/sfml-window-2.dll"
            $<TARGET_FILE_DIR:Gaianox>)
    add_custom_command(TARGET Gaianox POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$ENV{SFML_ROOT}/bin/sfml-audio-2.dll"
            $<TARGET_FILE_DIR:Gaianox>)
    add_custom_command(TARGET Gaianox POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$ENV{SFML_ROOT}/bin/sfml-system-2.dll"
            $<TARGET_FILE_DIR:Gaianox>)

    # Debug DLLs (with -d)
    add_custom_command(TARGET Gaianox POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$ENV{SFML_ROOT}/bin/sfml-graphics-d-2.dll"
            $<TARGET_FILE_DIR:Gaianox>)
    add_custom_command(TARGET Gaianox POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$ENV{SFML_ROOT}/bin/sfml-window-d-2.dll"
            $<TARGET_FILE_DIR:Gaianox>)
    add_custom_command(TARGET Gaianox POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$ENV{SFML_ROOT}/bin/sfml-audio-d-2.dll"
            $<TARGET_FILE_DIR:Gaianox>)
    add_custom_command(TARGET Gaianox POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$ENV{SFML_ROOT}/bin/sfml-system-d-2.dll"
            $<TARGET_FILE_DIR:Gaianox>)
endif()
