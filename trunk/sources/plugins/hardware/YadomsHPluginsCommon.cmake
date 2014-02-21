# Macro for setting up a plugin
#
#   ADD_HARDWARE_PLUGIN(target [sourcefile1.cpp sourcefile2.cpp ...])
#
# It configure the plugin to link as a module, with boost

MACRO(ADD_HARDWARE_PLUGIN _targetName)
	MESSAGE(STATUS "Hardware plugin precompiled header dir = ${HPLUGIN_PRECOMPILED_HEADER_DIR}")

	add_library(${_targetName} MODULE ${HARDWARE_PLUGIN_BASE_HEADER_SRC} ${HPLUGIN_PRECOMPILED_HEADER_DIR}/stdafx.h ${HPLUGIN_PRECOMPILED_HEADER_DIR}/stdafx.cpp ${ARGN})
	set_property( TARGET ${_targetName} PROPERTY INCLUDE_DIRECTORIES ${HPLUGIN_PRECOMPILED_HEADER_DIR} ${SHARED_INCL_DIR} ${BOOST_INCL_DIR} )
	target_link_libraries(${_targetName} yadoms-shared ${LIBS} ${CMAKE_DL_LIBS})

	IF(WIN32 OR XCODE)
		SET_PROPERTY(TARGET ${_targetName} PROPERTY FOLDER "Plugins")
	ENDIF()
ENDMACRO()
