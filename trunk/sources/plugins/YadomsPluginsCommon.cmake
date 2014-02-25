# Macro for setting up a plugin
#
#   ADD_PLUGIN(target [sourcefile1.cpp sourcefile2.cpp ...])
#
# It configure the plugin to link as a module, with boost

MACRO(PLUGIN_SOURCES _targetName)
	add_library(${_targetName} MODULE ${ARGN})
	
	IF(MSVC OR XCODE)
		SET_PROPERTY(TARGET ${_targetName} PROPERTY FOLDER "plugins")
	ENDIF()
ENDMACRO()

MACRO(PLUGIN_INCLDIR _targetName)
	set_property( TARGET ${_targetName} PROPERTY INCLUDE_DIRECTORIES ${SHARED_INCL_DIR} ${BOOST_INCL_DIR} ${ARGN})
ENDMACRO()

MACRO(PLUGIN_LINK _targetName)
	target_link_libraries(${_targetName} yadoms-shared ${LIBS} ${CMAKE_DL_LIBS} ${ARGN})
ENDMACRO()

