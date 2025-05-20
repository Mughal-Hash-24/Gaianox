cmake_minimum_required(VERSION 3.20)
project(Gaianox)

set(CMAKE_CXX_STANDARD 17)

# Path to SFML (change slashes to forward or escape backslashes)
set(SFML_DIR "C:/SFML-2.6.2/lib/cmake/SFML")

# Include SFML components
find_package(SFML 2.6 COMPONENTS graphics window audio system REQUIRED)

# Add all your source files here
add_executable(Gaianox
        Gaianox/main.cpp
        Gaianox/Utilities.h
        Gaianox/Animation.h
        Gaianox/Block.h
        Gaianox/Chunk.h
        Gaianox/TextureManager.h
)

# Include SFML headers
target_include_directories(Gaianox PRIVATE "C:/SFML-2.6.2/include")

# Link SFML libraries (use -d suffix for Debug builds)
target_link_libraries(Gaianox
        sfml-graphics
        sfml-window
        sfml-audio
        sfml-system
)
