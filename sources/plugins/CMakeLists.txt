# define all plugins library

set(HARDWARE_PLUGIN_BASE_HEADER_SRC
hardware/IHardwarePlugin.h
)

add_library(fakePlugin MODULE ${HARDWARE_PLUGIN_BASE_HEADER_SRC} hardware/FakePlugin/FakePlugin.cpp hardware/FakePlugin/FakePlugin.h)
target_link_libraries(fakePlugin ${LIBS} ${CMAKE_DL_LIBS})
IF(MSVC)
	SET_PROPERTY(TARGET fakePlugin PROPERTY FOLDER "Plugins")
ENDIF()