# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

########################
## FakePlugin
########################
set(FAKE_PLUGING_SRC
	FakePlugin.cpp 
	FakePlugin.h
	FakePluginConfiguration.cpp 
	FakePluginConfiguration.h
)


ADD_PLUGIN(fakePlugin ${FAKE_PLUGING_SRC})
