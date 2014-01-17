# include common plugin definitions
include("YadomsPlugins.cmake")

########################
## FakePlugin
########################
set(FAKE_PLUGING_SRC
	hardware/FakePlugin/FakePlugin.cpp 
	hardware/FakePlugin/FakePlugin.h
  
	../shared/shared/HardwarePluginConfiguration/HardwarePluginConfiguration.hpp
	../shared/shared/HardwarePluginConfiguration/HardwarePluginConfigurationParameters.hpp
)
	
ADD_PLUGIN(fakePlugin ${FAKE_PLUGING_SRC})

