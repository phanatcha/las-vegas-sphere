cmake_minimum_required(VERSION 3.5)
cmake_policy(VERSION 3.5)

project(LasVegasSphere)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(GLM REQUIRED)
find_package(GLFW3 REQUIRED)

find_library(COCOA_LIBRARY Cocoa)
find_library(OpenGL_LIBRARY OpenGL)
find_library(IOKit_LIBRARY IOKit)
find_library(CoreVideo_LIBRARY CoreVideo)
mark_as_advanced(COCOA_LIBRARY OpenGL_LIBRARY)

include_directories(${CMAKE_SOURCE_DIR}/includes)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/include)


add_executable(${PROJECT_NAME} 
    las-vegas-sphere/main.cpp       
    las-vegas-sphere/Sphere.cpp  
    las-vegas-sphere/Cylinder.cpp
    glad.c           
    stb_image.cpp    
)

target_link_libraries(${PROJECT_NAME}
    ${COCOA_LIBRARY} 
    ${IOKit_LIBRARY} 
    ${OpenGL_LIBRARY} 
    ${CoreVideo_LIBRARY}
    ${GLFW3_LIBRARY}
)

file(GLOB SHADERS 
    "${CMAKE_SOURCE_DIR}/*.vs" 
    "${CMAKE_SOURCE_DIR}/*.fs"
    "${CMAKE_SOURCE_DIR}/las-vegas-sphere/*.vs" 
    "${CMAKE_SOURCE_DIR}/las-vegas-sphere/*.fs"
)

foreach(SHADER ${SHADERS})
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SHADER}"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    )
endforeach()