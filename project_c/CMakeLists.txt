cmake_minimum_required(VERSION 3.22)
project(app C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/thirdparty/glfw/include
    ${CMAKE_SOURCE_DIR}/thirdparty/glad/include
    ${CMAKE_SOURCE_DIR}/thirdparty/cglm/include
    ${CMAKE_SOURCE_DIR}/thirdparty/stb/
    ${CMAKE_SOURCE_DIR}/thirdparty/freetype/include
    ${CMAKE_SOURCE_DIR}/thirdparty/openal-soft/include/
)

# Add sources
add_executable(app
    src/main.c
    src/window.c
    src/shader.c
    src/image.c

    thirdparty/glad/src/glad.c
    thirdparty/stb/stb_image.c
)

# Add GLFW (requires source code inside thirdparty/glfw)
add_subdirectory(thirdparty/glfw)

# Link dependencies
find_package(Threads REQUIRED)

target_link_libraries(app
    PRIVATE
    glfw
    Threads::Threads
    m
    dl
    freetype
)

# More robust copy command that creates directory first
add_custom_target(copy_content ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/content
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/content
        ${CMAKE_BINARY_DIR}/content
    COMMENT "Copying content folder to build directory"
    VERBATIM
)

# Make app depend on the copy operation
add_dependencies(app copy_content)
