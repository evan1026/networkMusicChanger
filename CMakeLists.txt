cmake_minimum_required(VERSION 2.8)

project(networkMusicChanger)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

set(SOURCE_DIR "src")

set(NMC_VERSION_MAJOR 0)
set(NMC_VERSION_MINOR 1)
set(NMC_VERSION_PATCH 0)

add_definitions( -DNMC_VERSION="NetworkMusicChanger version ${NMC_VERSION_MAJOR}.${NMC_VERSION_MINOR}.${NMC_VERSION_PATCH}" )

set(CMAKE_MODULE_PATH "/usr/local/share/Logger/;/usr/local/share/SFML/cmake/Modules/;${CMAKE_MODULE_PATH}")

set(EXECUTABLE_1_NAME "networkMusicCLI")
set(EXECUTABLE_2_NAME "networkMusicServer")

add_executable(${EXECUTABLE_1_NAME}
               	${SOURCE_DIR}/client/main.cpp)
add_executable(${EXECUTABLE_2_NAME}
                ${SOURCE_DIR}/server/main.cpp)

find_package(SFML 2 REQUIRED system network)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_1_NAME} ${SFML_LIBRARIES})
  target_link_libraries(${EXECUTABLE_2_NAME} ${SFML_LIBRARIES})
endif()

find_package(Logger REQUIRED)
target_link_libraries(${EXECUTABLE_1_NAME} Logger)
target_link_libraries(${EXECUTABLE_2_NAME} Logger)

