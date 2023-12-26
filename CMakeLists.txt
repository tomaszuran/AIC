cmake_minimum_required(VERSION 3.25.1)

project(AIC C)

#set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS}") # -Wall -Wextra -Wpedantic  -Wno-unused-function -Wno-unused-parameter -Wno-missing-field-initializers")

SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -Wpedantic  -Wno-unused-function -Wno-unused-parameter -Wno-missing-field-initializers")
SET(GCC_COVERAGE_LINK_FLAGS  "" )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}\\bin)
SET(CMAKE_C_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

set(SOURCES src/main.c
            src/matrix.c
            src/serialize.c)


add_executable(AIC ${SOURCES})
target_include_directories(AIC PRIVATE "inc/")

