# Macro for setting up a plugin
#
#   ADD_PLUGIN(target [sourcefile1.cpp sourcefile2.cpp ...])
#
# It configure the plugin to link as a module, with boost

# define all plugins library
set(HARDWARE_PLUGIN_BASE_HEADER_SRC
hardware/IHardwarePlugin.h
hardware/IHardwarePluginInformation.h
hardware/HardwarePluginInformation.hpp
hardware/HardwarePluginImplementationHelper.h
)


MACRO(ADD_PLUGIN _targetName)
	add_library(${_targetName} MODULE ${HARDWARE_PLUGIN_BASE_HEADER_SRC} ${ARGN})
	set_property( TARGET ${_targetName} PROPERTY INCLUDE_DIRECTORIES ${TOOLS_INCL_DIR} ${BOOST_INCL_DIR})
	target_link_libraries(${_targetName} yadoms-tools ${LIBS} ${CMAKE_DL_LIBS})
	IF(MSVC)
		SET_PROPERTY(TARGET ${_targetName} PROPERTY FOLDER "Plugins")
	ENDIF()
ENDMACRO()