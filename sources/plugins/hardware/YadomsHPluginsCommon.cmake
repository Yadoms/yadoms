# Macro for setting up a plugin
#
#   ADD_HARDWARE_PLUGIN(target [sourcefile1.cpp sourcefile2.cpp ...])
#
# It configure the plugin to link as a module, with boost

MACRO(ADD_HARDWARE_PLUGIN _targetName)
	add_library(${_targetName} MODULE ${HARDWARE_PLUGIN_BASE_HEADER_SRC} ${ARGN})
	set_property( TARGET ${_targetName} PROPERTY INCLUDE_DIRECTORIES ${SHARED_INCL_DIR} ${BOOST_INCL_DIR})
	target_link_libraries(${_targetName} yadoms-shared ${LIBS} ${CMAKE_DL_LIBS})
	IF(MSVC)
		SET_PROPERTY(TARGET ${_targetName} PROPERTY FOLDER "Plugins")
	ENDIF()
ENDMACRO()