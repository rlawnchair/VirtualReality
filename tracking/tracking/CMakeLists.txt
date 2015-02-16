project(rv_tracking)

cmake_minimum_required(VERSION 2.6)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(OpenCV REQUIRED)
find_package(VRPN REQUIRED)

include_directories(${OpenCV2_INCLUDE_DIRS})
include_directories(${VRPN_INCLUDES})
include_directories(${QUAT_INCLUDES})

set(SRCS src/interactionServer.cpp src/vrpnTrackerColors.hpp src/vrpnTrackerColors.cpp)

add_executable(interactionServer ${SRCS})

target_link_libraries(interactionServer ${OpenCV2_LIBRARIES}
${VRPN_LIBRARY} pthread)

if(APPLE)
  target_link_libraries(interactionServer "-framework Foundation" "-framework Cocoa")
endif()
