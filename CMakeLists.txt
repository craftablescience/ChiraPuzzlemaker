cmake_minimum_required(VERSION 3.15)
project(ChiraEditor)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
set(CMAKE_INSTALL_RPATH $ORIGIN)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

string(TOLOWER "${CMAKE_CXX_COMPILER_ID}" COMPILER_ID)

if(UNIX OR MINGW)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

# Chira Engine
set(CHIRA_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CHIRA_BUILD_WITH_WARNINGS OFF CACHE BOOL "" FORCE)
set(CHIRA_BUILD_EDITOR OFF CACHE BOOL "" FORCE)
add_subdirectory(external/engine)

# SpeedyKeyV
add_subdirectory(external/speedykv)

# VTFLib
set(USE_LIBTXC_DXTN OFF CACHE BOOL "" FORCE)
add_subdirectory(external/VTFLib)

include(src/loader/CMakeLists.txt)
include(src/render/CMakeLists.txt)
include(src/resource/CMakeLists.txt)
list(APPEND EDITOR_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/editor.cpp)
if(WIN32)
    list(APPEND EDITOR_SOURCES ${CMAKE_CURRENT_LIST_DIR}/resources/win32/project.rc)
endif()

add_executable(${PROJECT_NAME} ${EDITOR_SOURCES})
target_link_libraries(${PROJECT_NAME} ChiraEngine ${OPENGL_LIBRARIES} ${VTFLIB_LIBRARIES} VTFLib13 keyvalues)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${VTFLIB_INCLUDE_DIRS})

# Definitions
target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD2)
if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DDEBUG)
    if(COMPILER_ID MATCHES "clang")
        target_compile_options(${PROJECT_NAME} PUBLIC -fno-limit-debug-info)
    endif()
    if(UNIX OR MINGW)
        # On UNIX, build with debug friendly optimizations and debug symbols
        # On windows, just disable optimizations entirely
        target_compile_options(${PROJECT_NAME} PUBLIC -Og -g)
    elseif(MSVC)
        target_compile_options(${PROJECT_NAME} PUBLIC /Od)
    endif()
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DRELEASE)
    # Build with max optimizations and don't omit stack ptr for debugging reasons
    if(UNIX OR MINGW)
        target_compile_options(${PROJECT_NAME} PUBLIC -O3 -fno-omit-frame-pointer)
    elseif(MSVC)
        target_compile_options(${PROJECT_NAME} PUBLIC /O2)
    endif()
endif()

# Resources provided by the engine
add_custom_target(copyEngineResources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/external/engine/resources/engine/
        ${CMAKE_BINARY_DIR}/resources/engine/)
add_dependencies(${PROJECT_NAME} copyEngineResources)

# Resources provided by the editor
add_custom_target(copyEditorResources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/
        ${CMAKE_BINARY_DIR}/resources/editor/)
add_dependencies(${PROJECT_NAME} copyEditorResources)
