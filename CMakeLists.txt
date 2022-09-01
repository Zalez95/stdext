cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
include(GNUInstallDirs)

###############################################################################
# 									STDEXT
###############################################################################
# Project name and version
project(stdext LANGUAGES CXX)
set(STDEXT_VERSION_MAJOR "1")
set(STDEXT_VERSION_MINOR "0")
set(STDEXT_VERSION "${STDEXT_VERSION_MAJOR}.${STDEXT_VERSION_MINOR}")

# Find the library source files
file(GLOB_RECURSE STDEXT_SOURCES "src/*.cpp")

# Create the library
add_library(stdext ${STDEXT_SOURCES})

# Add the target properties
target_include_directories(stdext
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE "src"
)
set_target_properties(stdext PROPERTIES
	CXX_STANDARD			17
	CXX_STANDARD_REQUIRED	On
)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(stdext PRIVATE "-Wall" "-Wextra" "-Wpedantic")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	target_compile_options(stdext PRIVATE "/W4" "/bigobj" "-D_CRT_SECURE_NO_WARNINGS")
endif()

# Install the target
install(TARGETS stdext EXPORT stdextTargets)
install(EXPORT stdextTargets
FILE stdextTargets.cmake
	NAMESPACE stdext::
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/stdext"
)
install(DIRECTORY "include/stdext" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
