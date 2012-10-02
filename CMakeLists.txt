project(dlo)
cmake_minimum_required(VERSION 2.8)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	message("creating Debug-Version")
	add_definitions( -DDEBUG )
	set( CMAKE_CXX_FLAGS "-std=c++11 -O0 -Wall -Wextra -pedantic -Werror -g" )
elseif(CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
	message("creating Release-Version with Debug-information")
	add_definitions( -DDEBUG )
	set( CMAKE_CXX_FLAGS "-std=c++11 -O3 -Wall -Wextra -pedantic -Werror -g" )
else()
	message("creating Release-Version")
	set( CMAKE_CXX_FLAGS "-std=c++11 -O3 -Wall -Wextra -pedantic -Werror" )
endif()

option(BUILD_TESTS "build-tests" OFF)
option(BUILD_SHARED "build-shared" ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(src)
