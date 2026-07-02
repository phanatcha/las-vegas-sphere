cmake_minimum_required(VERSION 3.10)

project(LasVegasSphere)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(/opt/homebrew/opt/openal-soft/include)
link_directories(/opt/homebrew/opt/openal-soft/lib)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(GLFW3 REQUIRED)

configure_file(
    "${CMAKE_SOURCE_DIR}/configuration/root_directory.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/root_directory.h"
)

file(GLOB SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
    "*.cpp" 
    "*.c"   
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/includes
    ${CMAKE_SOURCE_DIR}/dependencies/include
    ${CMAKE_CURRENT_BINARY_DIR}
)


target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    ${GLFW3_LIBRARY}
    /opt/homebrew/opt/openal-soft/lib/libopenal.dylib
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
)

file(GLOB SHADERS 
    "${CMAKE_SOURCE_DIR}/*.vs" 
    "${CMAKE_SOURCE_DIR}/*.fs"
    "${CMAKE_SOURCE_DIR}/src/*.vs" 
    "${CMAKE_SOURCE_DIR}/src/*.fs"
    "${CMAKE_SOURCE_DIR}/resources/shaders/*.vs"
    "${CMAKE_SOURCE_DIR}/resources/shaders/*.fs"
)

foreach(SHADER ${SHADERS})
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SHADER}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
        COMMENT "Copying shader to bin: ${SHADER}" 
    )
endforeach()