cmake_minimum_required(VERSION 3.18)
project(gb)
set(PROJECT_NAME "krystal.x86_64")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenAL REQUIRED)
find_package(SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED)
find_package(PhysFS REQUIRED)
find_package(Lua REQUIRED)

include_directories(${SDL2_DIRS})
#include_directories(${SDL2_IMAGE_DIRS})
include_directories(${PhysFS_DIRS})
include_directories(${LUA_INCLUDE_DIR})


file(GLOB_RECURSE SRCS "src/*.h" "src/*.c")  
add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} PRIVATE ${SDL2_IMAGE_LIBRARIES} PRIVATE ${SDL2_MIXER_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PHYSFS_LIBRARY})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENAL_LIBRARY})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIBRARY})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_image::SDL2_image)
