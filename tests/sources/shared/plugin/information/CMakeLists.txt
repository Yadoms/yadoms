
IF(NOT DISABLE_TEST_PLUGIN_INFORMATION)
   ADD_YADOMS_SOURCES(
      shared/shared/StringExtension.h
      shared/shared/StringExtension.cpp
      shared/shared/plugin/information/IInformation.h
      shared/shared/plugin/information/Information.h
      shared/shared/plugin/information/Information.cpp)
   ADD_SOURCES(TestPluginInformation.cpp)
   
ENDIF()
