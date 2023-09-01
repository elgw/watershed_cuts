set(CMAKE_BUILD_WITH_INSTALL_RPATH 0)
add_executable(DIPlib_watershed DIPlib_watershed.cpp)
target_include_directories(DIPlib_watershed
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/)
target_link_libraries(DIPlib_watershed
  DIP
  ${CMAKE_CURRENT_SOURCE_DIR}/../watershed_cuts.o)
