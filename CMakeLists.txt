cmake_minimum_required(VERSION 3.21)
project(HappyBird LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# On Windows, default SFML to C:/SFML if not overridden via -DSFML_DIR=...
if(WIN32 AND NOT DEFINED SFML_DIR)
    set(SFML_DIR "C:/SFML/lib/cmake/SFML")
endif()

find_package(SFML 3.0 REQUIRED COMPONENTS Graphics Audio)

add_executable(HappyBird
    HappyBird/HappyBird/Bird.cpp
    HappyBird/HappyBird/HappyBird.cpp
    HappyBird/HappyBird/HappyBirdTest.cpp
    HappyBird/HappyBird/Obstacle.cpp
)

target_include_directories(HappyBird PRIVATE HappyBird/HappyBird)
target_link_libraries(HappyBird PRIVATE SFML::Graphics SFML::Audio)

# Copy game assets to build output directory
file(GLOB GAME_ASSETS "${CMAKE_SOURCE_DIR}/HappyBird/Debug/*")
foreach(ASSET_PATH ${GAME_ASSETS})
    get_filename_component(ASSET_EXT  ${ASSET_PATH} EXT)
    get_filename_component(ASSET_NAME ${ASSET_PATH} NAME)
    string(TOLOWER "${ASSET_EXT}" ASSET_EXT_LOWER)
    if(NOT IS_DIRECTORY "${ASSET_PATH}"
       AND NOT ASSET_EXT_LOWER STREQUAL ".exe"
       AND NOT ASSET_EXT_LOWER STREQUAL ".ilk"
       AND NOT ASSET_EXT_LOWER STREQUAL ".pdb"
       AND NOT ASSET_EXT_LOWER STREQUAL ".dll")
        configure_file("${ASSET_PATH}" "${CMAKE_BINARY_DIR}/${ASSET_NAME}" COPYONLY)
    endif()
endforeach()

# Copy SFML DLLs on Windows only (Linux uses system shared libraries)
if(WIN32)
    get_filename_component(SFML_BIN_DIR "${SFML_DIR}/../../../bin" ABSOLUTE)
    file(GLOB SFML_DLLS "${SFML_BIN_DIR}/*.dll")
    foreach(DLL_PATH ${SFML_DLLS})
        get_filename_component(DLL_NAME ${DLL_PATH} NAME)
        configure_file("${DLL_PATH}" "${CMAKE_BINARY_DIR}/${DLL_NAME}" COPYONLY)
    endforeach()
endif()
