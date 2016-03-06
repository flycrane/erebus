set(SOURCE
	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_manager.cc
	${CMAKE_CURRENT_SOURCE_DIR}/dynamic_library.cc
	PARENT_SCOPE
)

set(HEADER
	${HEADER}
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_manager.h
	${CMAKE_CURRENT_SOURCE_DIR}/iobject_adapater.h
	${CMAKE_CURRENT_SOURCE_DIR}/dynamic_library.h
	${CMAKE_CURRENT_SOURCE_DIR}/plugin.h
	PARENT_SCOPE
)	
