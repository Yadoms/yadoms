
IF(NOT DISABLE_TEST_PLUGIN_INFORMATION)
   ADD_YADOMS_SOURCES(
      shared/shared/StringExtension.h
      shared/shared/StringExtension.cpp
      shared/shared/plugin/information/IInformation.h
      server/pluginSystem/Information.h
      server/pluginSystem/Information.cpp)
   ADD_SOURCES(TestPluginInformation.cpp)
   
ENDIF()
