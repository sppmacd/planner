cmake_minimum_required(VERSION 2.6)
project(BsPlanner)

add_subdirectory(cli)

set(SOURCES
	"Assertions.cpp"
	"ByteStream.cpp"
	"Group.cpp"
	"GroupType.cpp"
	"main.cpp"
	"Planner.cpp"
	"Task.cpp"
	"TaskList.cpp"
	"Time.cpp"
)

add_executable(bspl ${SOURCES})
target_link_libraries(bspl bspl-cli readline)
target_include_directories(bspl PUBLIC ${CMAKE_SOURCE_DIR})
