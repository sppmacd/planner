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

add_executable(bsplanner ${SOURCES})
target_link_libraries(bsplanner bsplanner-cli readline)
target_include_directories(bsplanner PUBLIC ${CMAKE_SOURCE_DIR})
