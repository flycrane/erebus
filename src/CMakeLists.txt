add_subdirectory(plugin_framework)
add_subdirectory(plugins)
add_subdirectory(utils)
add_subdirectory(exceptions)
add_subdirectory(core)

set(SOURCE
	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/application.cc
	${CMAKE_CURRENT_SOURCE_DIR}/logger.cc
	PARENT_SCOPE
)

set(HEADER
	${HEADER}
	${CMAKE_CURRENT_SOURCE_DIR}/application.h
	${CMAKE_CURRENT_SOURCE_DIR}/idisassembler.h
	${CMAKE_CURRENT_SOURCE_DIR}/byte_order.h
	${CMAKE_CURRENT_SOURCE_DIR}/logger.h
	PARENT_SCOPE
)	
