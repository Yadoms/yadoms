# Macro for setting up a test
#
#   GENERATE_TEST_EXECUTABLE(target [sourcefile1.cpp sourcefile2.cpp ...])
#
# It generate an executable for the target test

MACRO(GENERATE_TEST_EXECUTABLE _targetName _base_SRC)

	MESSAGE(STATUS "Test precompiled header dir = ${TEST_PRECOMPILED_HEADER_DIR}")

	add_executable(${_targetName} ${_base_SRC} ${YADOMS_SRC})
	set_property(   TARGET ${_targetName} 
			PROPERTY INCLUDE_DIRECTORIES ${TEST_PRECOMPILED_HEADER_DIR} ${SHARED_INCL_DIR} ${BOOST_INCL_DIR} ${YADOMS_INCL_DIR})
	target_link_libraries(${_targetName} ${LIBS} ${CMAKE_DL_LIBS})

	#IF(MSVC OR XCODE)
	#	SET_PROPERTY(TARGET ${_targetName} PROPERTY FOLDER "Plugins")
	#ENDIF()
ENDMACRO()
