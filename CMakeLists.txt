#
# BFS project build script
#
cmake_minimum_required(VERSION 3.5.0)
project (BreadthFirstSearch)

set (CMAKE_CXX_STANDARD 14)

include_directories(./include)

file (GLOB HEADER_FILES
	"./include/*.h"
	"./include/app/*.h"
)

file (GLOB AUXILIARY_FILES
	"./include/auxiliary/*.h"
)
	
file (GLOB SOURCE_FILES
	"./src/*.cpp"
	"./src/*.h"
)

add_executable (${PROJECT_NAME} 
	${SOURCE_FILES}
	${HEADER_FILES}
	${AUXILIARY_FILES}
)

source_group(src FILES ${SOURCE_FILES})
source_group(include FILES ${HEADER_FILES})
source_group(auxiliary FILES ${AUXILIARY_FILES})
