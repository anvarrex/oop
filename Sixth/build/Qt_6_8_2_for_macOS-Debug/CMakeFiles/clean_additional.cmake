# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/Sixth_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Sixth_autogen.dir/ParseCache.txt"
  "Sixth_autogen"
  )
endif()
