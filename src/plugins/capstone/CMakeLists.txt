include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CAPSTONE_PLUGIN_SOURCE
	${CMAKE_CURRENT_SOURCE_DIR}/capstone_plugin.cc
	${CMAKE_CURRENT_SOURCE_DIR}/capstone_x86_disassembler.cc
)

set(CAPSTONE_PLUGIN_HEADER
	${CMAKE_CURRENT_SOURCE_DIR}/capstone_plugin.h
	${CMAKE_CURRENT_SOURCE_DIR}/capstone_x86_disassembler.h
)

add_library(capstone_plugin SHARED ${CAPSTONE_PLUGIN_HEADER} ${CAPSTONE_PLUGIN_SOURCE})
