set(SOURCES
	"commands.cpp"
	"config.cpp"
	"utils.cpp"
)

add_library(bspl-cli ${SOURCES})
target_include_directories(bspl-cli PUBLIC ${CMAKE_SOURCE_DIR})
