cmake_minimum_required(VERSION 3.10)
project(LunaLang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    src/lexer.cpp
    src/token.cpp
    src/util.cpp
    src/ast.cpp
    src/parser.cpp
    src/interpreter.cpp
    src/value.cpp
    src/main.cpp
    src/math_lib.cpp
    src/string_lib.cpp
    src/error.cpp
    src/time_lib.cpp
    src/vec_lib.cpp
    src/env.cpp
    src/library.cpp
    src/file_lib.cpp
    src/list_lib.cpp
)

add_executable(luna ${SOURCES})

# Include directories (target-specific)
target_include_directories(luna PRIVATE include)

# MSVC-specific warnings suppression
if (MSVC)
    target_compile_definitions(luna PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# Link math library on Linux
if (UNIX AND NOT APPLE)
    target_link_libraries(luna PRIVATE m)
endif()

# Output directory
set_target_properties(luna PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
