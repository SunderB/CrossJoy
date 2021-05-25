cmake_minimum_required(VERSION 3.18)
project(CrossJoy)

list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake
)

find_package(HIDAPI REQUIRED)

set(SOURCE_FILES
  "src/main.cpp"
  "src/joycon.cpp"
  "src/joycon.h"
  "src/utils.cpp"
  "src/utils.h"
)

add_executable(joycon ${SOURCE_FILES})
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  target_link_libraries(joycon
    HIDAPI::hidapi-hidraw
  )
else()
  target_link_libraries(joycon
    HIDAPI::hidapi
  )
endif()
