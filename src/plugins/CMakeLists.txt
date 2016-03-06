set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/plugins)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(capstone)

set(SOURCE
	${SOURCE}
	PARENT_SCOPE
)

set(HEADER
	${HEADER}
	PARENT_SCOPE
)
