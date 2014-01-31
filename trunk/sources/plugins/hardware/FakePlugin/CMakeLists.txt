# include common plugin definitions
include("../YadomsHPluginsCommon.cmake")

########################
## FakePlugin
########################
set(FAKE_PLUGING_SRC
	FakePlugin.cpp 
	FakePlugin.h
	FakePluginConfiguration.cpp 
	FakePluginConfiguration.h
)


ADD_HARDWARE_PLUGIN(fakePlugin ${FAKE_PLUGING_SRC})
