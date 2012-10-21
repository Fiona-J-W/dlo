project(dlo)
cmake_minimum_required(VERSION 2.8)

#we need C++11:
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

#Get as many warnings as possible:
if( CMAKE_COMPILER_IS_GNUCXX )
	message("using g++")
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror -Weffc++" )
elseif( CMAKE_COMPILER_IS_CLANGXX )
	message("using clang++")
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Werror" )
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	message("creating Debug-Version")
	add_definitions( -DDEBUG )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g" )
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
	message("creating Release-Version")
	add_definitions( -DNDEBUG )
	if( CMAKE_COMPILER_IS_GNUCXX )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3"  )
	elseif( CMAKE_COMPILER_IS_CLANGXX )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O4"  )
	endif()
endif()

option(BUILD_TESTS "build-tests" ON)
option(BUILD_SHARED "build-shared" ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(src)
