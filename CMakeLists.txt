cmake_minimum_required( VERSION 2.8 )
project (ComputedTree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnon-template-friend -std=c++0x -msse2 -fPIC")


## TCMALLOC used to optimized memory allocation
find_package(PkgConfig REQUIRED)
pkg_check_modules(TCMALLOC libtcmalloc_minimal)
if(TCMALLOC_FOUND)
  include_directories(${TCMALLOC_INCLUDE_DIRS})
  link_directories(${TCMALLOC_LIBRARY_DIRS})
  list(APPEND MG_LIBRARIES ${TCMALLOC_LIBRARIES})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free")
endif(TCMALLOC_FOUND)

foreach(p LIB INCLUDE)
	set(var CMAKE_INSTALL_${p}DIR)
	if(NOT IS_ABSOLUTE "${${var}}")
		set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
	endif()
endforeach()

#### Package findings #####
find_package(PkgConfig REQUIRED)

## TCMALLOC used to optimized memory allocation
pkg_check_modules(TCMALLOC libtcmalloc_minimal)
if(TCMALLOC_FOUND)
# message("TC MALLOC Found")
  include_directories(${TCMALLOC_INCLUDE_DIRS})
  link_directories(${TCMALLOC_LIBRARY_DIRS})
  list(APPEND MG_LIBRARIES ${TCMALLOC_LIBRARIES})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free")
elseif(TCMALLOC_FOUND)
message("TC MALLOC not Found")
endif(TCMALLOC_FOUND)

# Get the current working branch
execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_definitions("-DGIT_BRANCH=\"${GIT_BRANCH}\"")


set (INSTALL_LIB_PATH lib )

file( GLOB_RECURSE ComputedTree_SOURCES src/*.cpp )
FILE( GLOB_RECURSE ComputedTree_HEADERS include/*.h   include/*.hxx )

include_directories(	include
                        ${TCMALLOC_INCLUDE_DIRS}
                        )

link_directories(	${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}
                    ${TCMALLOC_LIBRARY_DIRS})

add_library(ComputedTree SHARED ${ComputedTree_SOURCES})

target_link_libraries(	ComputedTree
                        ${TCMALLOC_LIBRARIES}
                        )

add_executable (TestTree apps/test_tree.cpp)

target_link_libraries(	TestTree
 						ComputedTree
 						dl
						)


### installation of the files
SET(INSTALL_INCLUDE_DIRS ${CMAKE_INSTALL_INCLUDEDIR}/include/ComputedTree)

### Configure file
get_property(Include_Directories DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
SET(ComputedTree_INCLUDE_DIRS ${Include_Directories} ${INSTALL_INCLUDE_DIRS} CACHE INTERNAL "")
SET(ComputedTree_FLAGS ${CMAKE_CXX_FLAGS} CACHE INTERNAL "")
SET(ComputedTree_FOUND "TRUE" CACHE INTERNAL "")

configure_file(ComputedTreeConfig.cmake.in "${PROJECT_BINARY_DIR}/ComputedTreeConfig.cmake" @ONLY)

configure_file ("${PROJECT_SOURCE_DIR}/config_ComputedTree.h.in"
                "${PROJECT_SOURCE_DIR}/include/config_ComputedTree.h" )

install( FILES ${ComputedTree_HEADERS} DESTINATION ${INSTALL_INCLUDE_DIRS})

install(FILES "${PROJECT_BINARY_DIR}/ComputedTreeConfig.cmake"
	DESTINATION "${INSTALL_LIB_PATH}/ComputedTreeConfig" COMPONENT dev )

