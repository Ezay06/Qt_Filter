# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "RelWithDebInfo")
  file(REMOVE_RECURSE
  "CMakeFiles\\Qt_Filter_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Qt_Filter_autogen.dir\\ParseCache.txt"
  "Qt_Filter_autogen"
  )
endif()
