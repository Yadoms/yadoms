# define all plugins library
add_library(fakePlugin MODULE  hardware/fakePlugin.cpp)
target_link_libraries(fakePlugin hardwarePluginBaseLib sharedLib)
SET_PROPERTY(TARGET fakePlugin   PROPERTY FOLDER "Plugins")