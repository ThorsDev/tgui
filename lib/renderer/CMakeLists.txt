cmake_minimum_required(VERSION 3.21)
project(tgui_renderer VERSION 1.2.3 LANGUAGES CXX)

include(GNUInstallDirs)

file(GLOB_RECURSE FILE_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

file(GLOB_RECURSE HEADER_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
	PREFIX "Source Files"
	FILES ${FILE_SOURCES}
)

source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}/include"
	PREFIX "Header Files"
	FILES ${HEADER_SOURCES}
)

add_library(tgui_renderer STATIC ${FILE_SOURCES} ${HEADER_SOURCES})
add_library(tgui::renderer ALIAS tgui_renderer)

target_include_directories(tgui_renderer
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

install(TARGETS tgui_renderer
  EXPORT tgui_renderer_targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tgui_renderer
)

install(EXPORT tgui_renderer_targets
  FILE tgui_renderer_targets.cmake
  NAMESPACE tgui::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tgui_renderer
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/tguiRendererConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
  "${CMAKE_CURRENT_LIST_DIR}/cmake/tguiRendererConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/tguiRendererConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tgui_renderer
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/tguiRendererConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/tguiRendererConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tguiRenderer
)
