cmake_minimum_required(VERSION 2.6)
project(SomethingLikeAn)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

# Set version information in a config.h file
set(SomethingLikeAn_VERSION_MAJOR 0)
set(SomethingLikeAn_VERSION_MINOR 1)
set(SomethingLikeAn_VERSION_PATCH 0)
configure_file(
  "${PROJECT_SOURCE_DIR}/src/config.h.in"
  "${PROJECT_BINARY_DIR}/config/config.h"
  )
include_directories("${PROJECT_BINARY_DIR}/config")

# Define sources and executable
set(EXECUTABLE_NAME "SomethingLikeAn")
add_executable(${EXECUTABLE_NAME} src/main.cpp
                                  src/SLA/Camera.cpp
                                  src/SLA/ClipSheet.cpp
                                  src/SLA/Animation.cpp
                                  src/SLA/Player.cpp
                                  src/SLA/CollisionLayer.cpp)
add_custom_command(TARGET SomethingLikeAn
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/data 
                                                              ${PROJECT_BINARY_DIR}/data)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()


# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)


# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${SomethingLikeAn_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${SomethingLikeAn_VERSION_MINOR}")
include(CPack)
