cmake_minimum_required(VERSION 3.16)
project(ChiraPuzzlemaker)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
set(CMAKE_INSTALL_RPATH $ORIGIN)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# VTFLib needs this
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Chira Engine
set(CHIRA_BUILD_TESTS           OFF CACHE BOOL "" FORCE)
set(CHIRA_BUILD_WITH_WARNINGS   OFF CACHE BOOL "" FORCE)
set(CHIRA_BUILD_EDITOR          OFF CACHE BOOL "" FORCE)
set(CHIRA_USE_STEAMWORKS        ON  CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(external/engine)

# SpeedyKeyV
add_subdirectory(external/speedykv)

# VTFLib
set(VTFLIB_STATIC OFF CACHE BOOL "" FORCE)
add_subdirectory(external/VTFLib)

# VPKTool
add_subdirectory(external/vpktool)

include(src/CMakeLists.txt)
if(WIN32)
    list(APPEND PUZZLEMAKER_SOURCES ${CMAKE_CURRENT_LIST_DIR}/resources/icon/icon.rc)
endif()
list(APPEND PUZZLEMAKER_SOURCES ${PUZZLEMAKER_HEADERS})
add_executable(${PROJECT_NAME} ${PUZZLEMAKER_SOURCES})
apply_optimizations(${PROJECT_NAME})
#target_precompile_headers(${PUZZLEMAKER_HEADERS})

target_link_libraries(${PROJECT_NAME} ChiraEngine ${OPENGL_LIBRARIES} ${VTFLIB_LIBRARIES} vtflib VPKTool keyvalues)
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/external/VTFLib/VTFLib
        ${CMAKE_CURRENT_SOURCE_DIR}/external/vpktool/include
        ${VTFLIB_INCLUDE_DIRS})

# Definitions
target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD2)
if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DDEBUG)
    if(CHIRA_COMPILER_CLANG)
        target_compile_options(${PROJECT_NAME} PUBLIC -fno-limit-debug-info)
    endif()
    if(UNIX OR MINGW)
        # On UNIX, build with debug friendly optimizations and debug symbols
        # On windows, just disable optimizations entirely
        target_compile_options(${PROJECT_NAME} PUBLIC -Og -g)
    elseif(CHIRA_COMPILER_MSVC)
        target_compile_options(${PROJECT_NAME} PUBLIC /Od)
    endif()
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DRELEASE)
    # Build with max optimizations and don't omit stack ptr for debugging reasons
    if(UNIX OR MINGW)
        target_compile_options(${PROJECT_NAME} PUBLIC -O3 -fno-omit-frame-pointer)
    elseif(CHIRA_COMPILER_MSVC)
        target_compile_options(${PROJECT_NAME} PUBLIC /O2)
    endif()
endif()

if(UNIX OR MINGW)
    target_compile_options(${PROJECT_NAME} PUBLIC -Wall)
elseif(CHIRA_COMPILER_MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /W4) # not all, because MSVC has loads of idiotic errors
endif()

# Resources provided by the editor
add_custom_target(CopyPuzzlemakerResources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/
        ${CMAKE_BINARY_DIR}/resources/puzzlemaker/)
add_dependencies(${PROJECT_NAME} CopyPuzzlemakerResources)
