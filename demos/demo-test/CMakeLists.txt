cmake_minimum_required(VERSION 3.21)
project(tgui_demo_test VERSION 1.2.3 LANGUAGES CXX)


file(GLOB_RECURSE FILE_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

file(GLOB_RECURSE HEADER_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
	PREFIX "Source Files"
	FILES ${FILE_SOURCES}
)

source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
	PREFIX "Header Files"
	FILES ${HEADER_SOURCES}
)

add_executable(tgui_demo_test ${FILE_SOURCES} ${HEADER_SOURCES})
target_link_libraries(tgui_demo_test PRIVATE
	tgui::tgui
	tgui::renderer
)