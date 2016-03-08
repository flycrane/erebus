set(SOURCE
	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/dynamic_link_library_exception.cc
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_exception.cc
	PARENT_SCOPE
)

set(HEADER
	${HEADER}
	${CMAKE_CURRENT_SOURCE_DIR}/dynamic_link_library_exception.h
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_exception.h
	PARENT_SCOPE
)
