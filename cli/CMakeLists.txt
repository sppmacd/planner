set(SOURCES
	"commands.cpp"
	"config.cpp"
	"utils.cpp"
)

add_library(bsplanner-cli ${SOURCES})
target_include_directories(bsplanner-cli PUBLIC ${CMAKE_SOURCE_DIR})
