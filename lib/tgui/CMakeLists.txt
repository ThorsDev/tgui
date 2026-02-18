cmake_minimum_required(VERSION 3.21)
project(tgui VERSION 1.2.3 LANGUAGES CXX)

CPMAddPackage("gh:wqking/eventpp#v0.1.3")
CPMAddPackage("gh:gabime/spdlog#v1.17.0")

## spdlog
target_compile_definitions(spdlog PUBLIC SPDLOG_USE_STD_FORMAT)

add_library(tgui_spdlog INTERFACE)
add_library(tgui::spdlog ALIAS tgui_spdlog)

target_include_directories(tgui_spdlog INTERFACE
  $<BUILD_INTERFACE:${spdlog_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/spdlog>
)

install(DIRECTORY "${spdlog_SOURCE_DIR}/include/"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/spdlog
)
## spdlog end

## eventpp
add_library(tgui_eventpp INTERFACE)
add_library(tgui::eventpp ALIAS tgui_eventpp)

target_include_directories(tgui_eventpp INTERFACE
  $<BUILD_INTERFACE:${eventpp_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/eventpp>
)

install(DIRECTORY "${eventpp_SOURCE_DIR}/include/"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/eventpp
)
## eventpp end

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

add_library(tgui STATIC ${FILE_SOURCES} ${HEADER_SOURCES})
add_library(tgui::tgui ALIAS tgui)

target_include_directories(tgui
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/vendor/include
)

target_link_libraries(tgui
PUBLIC
	tgui::spdlog
	tgui::eventpp
)

target_compile_definitions(tgui PUBLIC
	$<$<PLATFORM_ID:Windows>:TGUI_PLATFORM_WINDOWS>
	$<$<PLATFORM_ID:Linux>:TGUI_PLATFORM_LINUX>
	$<$<PLATFORM_ID:FreeBSD>:TGUI_PLATFORM_BSD>
	$<$<PLATFORM_ID:Android>:TGUI_PLATFORM_ANDROID>
	$<$<PLATFORM_ID:Darwin>:TGUI_PLATFORM_MACOS>
	$<$<PLATFORM_ID:iOS>:TGUI_PLATFORM_IOS>
)

install(TARGETS tgui tgui_eventpp tgui_spdlog
  EXPORT tgui_targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tgui
)

install(EXPORT tgui_targets
  FILE tgui_targets.cmake
  NAMESPACE tgui::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tgui
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/tguiConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
  "${CMAKE_CURRENT_LIST_DIR}/cmake/tguiConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/tguiConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tgui
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/tguiConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/tguiConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tgui
)
