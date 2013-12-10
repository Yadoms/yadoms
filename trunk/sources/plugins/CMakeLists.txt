# include common plugin definitions
include("YadomsPlugins.cmake")

########################
## FakePlugin
########################
set(FAKE_PLUGING_SRC
	hardware/FakePlugin/FakePlugin.cpp 
	hardware/FakePlugin/FakePlugin.h
)
	
ADD_PLUGIN(fakePlugin ${FAKE_PLUGING_SRC})

