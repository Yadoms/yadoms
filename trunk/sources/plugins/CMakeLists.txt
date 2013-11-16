# define all plugins library

set(HARDWARE_PLUGIN_BASE_HEADER_SRC
hardware/IHardwarePlugin.h
)

add_library(fakePlugin MODULE ${HARDWARE_PLUGIN_BASE_HEADER_SRC} hardware/FakePlugin.cpp hardware/FakePlugin.h)
target_link_libraries(fakePlugin sharedLib)
SET_PROPERTY(TARGET fakePlugin PROPERTY FOLDER "Plugins")