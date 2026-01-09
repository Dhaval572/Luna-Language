cmake_minimum_required(VERSION 3.10)
project(LunaLang)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")

# Set include directories - add the include directory to the search path
include_directories(include)

# Define source files
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

# Add executable
add_executable(luna ${SOURCES})

# Link math library
# On Windows, the math library is part of the standard library
if(UNIX AND NOT APPLE)
    target_link_libraries(luna m)
endif()

# Set output directory
set_target_properties(luna PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)