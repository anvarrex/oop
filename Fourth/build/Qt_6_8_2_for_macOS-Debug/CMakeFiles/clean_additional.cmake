# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/Fourth_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Fourth_autogen.dir/ParseCache.txt"
  "Fourth_autogen"
  )
endif()
