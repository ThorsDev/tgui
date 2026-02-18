cmake_minimum_required(VERSION 3.21)
project(tgui_platform VERSION 1.2.3 LANGUAGES CXX)

CPMAddPackage("gh:glfw/glfw#3.4")

## glfw
# Install GLFW headers (keeps <GLFW/glfw3.h>)
install(DIRECTORY "${glfw_SOURCE_DIR}/include/GLFW"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
## glfw end

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

add_library(tgui_platform STATIC ${FILE_SOURCES} ${HEADER_SOURCES})
add_library(tgui::platform ALIAS tgui_platform)

target_link_libraries(tgui_platform 
PRIVATE
	tgui::tgui

PUBLIC
	glfw
)

target_include_directories(tgui_platform
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

install(TARGETS tgui_platform glfw
  EXPORT tgui_renderer_targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tgui_platform
)

install(EXPORT tgui_renderer_targets
  FILE tgui_renderer_targets.cmake
  NAMESPACE tgui::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tgui_platform
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/tguiPlatformConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
  "${CMAKE_CURRENT_LIST_DIR}/cmake/tguiPlatformConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/tguiPlatformConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tgui_platform
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/tguiPlatformConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/tguiPlatformConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tgui_platform
)
