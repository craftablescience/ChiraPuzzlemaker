cmake_minimum_required(VERSION 3.19)
project(ChiraEditor)
set(CMAKE_CXX_STANDARD 17)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DRELEASE)
endif()

# Chira Engine
add_subdirectory(external/ChiraEngine/src/)

# VTFLib
set(USE_LIBTXC_DXTN OFF CACHE BOOL "" FORCE)
add_subdirectory(external/VTFLib/)

include(src/loader/CMakeLists.txt)
list(APPEND EDITOR_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp)
if(WIN32)
    list(APPEND EDITOR_SOURCES ${CMAKE_CURRENT_LIST_DIR}/resources/editor/win32/project.rc)
endif()

add_executable(${PROJECT_NAME} ${EDITOR_SOURCES})
target_link_libraries(${PROJECT_NAME} ChiraEngine ${VTFLIB_LIBRARIES} VTFLib13)
target_include_directories(${PROJECT_NAME} PUBLIC ${VTFLIB_INCLUDE_DIRS})

# Resources provided by the engine
add_custom_target(copyEngineResources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/external/ChiraEngine/src/resources/engine/
        ${CMAKE_BINARY_DIR}/resources/engine/)
add_dependencies(${PROJECT_NAME} copyEngineResources)

# Resources provided by the editor
add_custom_target(copyEditorResources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/editor/
        ${CMAKE_BINARY_DIR}/resources/editor/)
add_dependencies(${PROJECT_NAME} copyEditorResources)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
