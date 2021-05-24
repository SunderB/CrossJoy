project(JoyCon)

#find_package(libhidapi REQUIRED)

set(SOURCE_FILES
  "src/main.cpp"
  "src/joycon.cpp"
  "src/joycon.h"
  "src/utils.cpp"
  "src/utils.h"
)
add_executable(joycon ${SOURCE_FILES})
target_link_libraries(joycon hidapi-hidraw)
