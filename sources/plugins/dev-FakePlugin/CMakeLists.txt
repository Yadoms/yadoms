# include common plugin definitions
include("../YadomsPluginsCommon.cmake")

#########################################
## Place here all the plugin source files
#########################################
set(FAKE_PLUGIN_SOURCES
	stdafx.h
	stdafx.cpp
	FakePlugin.h
	FakePlugin.cpp 
	FakeConfigurableDevice.h
	FakeConfigurableDevice.cpp 
	FakeDynamicallyConfigurableDevice.h
	FakeDynamicallyConfigurableDevice.cpp 
	FakePluginConfiguration.h
	FakePluginConfiguration.cpp 
	FakeCounter.h
	FakeCounter.cpp 
	FakeSensor.h
	FakeSensor.cpp 
	FakeSwitch.h
	FakeSwitch.cpp 
	FakeController.h
	FakeController.cpp 
)

#########################################
## Place here all additional include directories
## (by default yadoms-shared and boost are included)
#########################################
set(FAKE_PLUGIN_INCLDIR
)

#########################################
## Place here all additional libraries to link with
## (by default yadoms-shared and boost are included)
#########################################
set(FAKE_PLUGIN_LINK
)

#########################################
## Place here all additional compiler flags
## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
#########################################
##add_definitions("-lwiringPi")

#########################################
## Always leave these lines to configure plugin correctly
## even if FAKE_PLUGIN_INCLDIR or FAKE_PLUGIN_LINK are empty
#########################################
PLUGIN_SOURCES(dev-fakePlugin ${FAKE_PLUGIN_SOURCES})
PLUGIN_INCLDIR(dev-fakePlugin ${FAKE_PLUGIN_ADDITIONAL_INC_DIR})
PLUGIN_LINK(dev-fakePlugin ${FAKE_PLUGIN_ADDITIONAL_LIB})

#########################################
## Post-build commands
## Copy files to build directory
## "package.json" and "icon.png" are needed, but you can add extra-files
#########################################
PLUGIN_POST_BUILD_COPY_FILE(dev-fakePlugin package.json)
PLUGIN_POST_BUILD_COPY_FILE(dev-fakePlugin icon.png)
PLUGIN_POST_BUILD_COPY_DIRECTORY(dev-fakePlugin locales)
