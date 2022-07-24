include(${CMAKE_CURRENT_SOURCE_DIR}/src/resource/provider/CMakeLists.txt)

list(APPEND PUZZLEMAKER_HEADERS
        ${CMAKE_CURRENT_LIST_DIR}/vtfTextureResource.h)

list(APPEND PUZZLEMAKER_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/vtfTextureResource.cpp)
