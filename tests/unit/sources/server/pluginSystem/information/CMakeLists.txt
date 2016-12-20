
IF(NOT DISABLE_TEST_PLUGIN_INFORMATION)
   SET(ALLFILES
      shared/shared/StringExtension.h
      shared/shared/plugin/information/IInformation.h
      shared/shared/versioning/ReleaseType.h
      shared/shared/versioning/ReleaseType.cpp
      shared/shared/versioning/Version.h
      shared/shared/versioning/Version.cpp
      server/pluginSystem/Information.h
      server/pluginSystem/Information.cpp
      server/tools/tools/SupportedPlatformsChecker.h
      server/tools/tools/SupportedPlatformsChecker.cpp
   )
   
   
   IF(WIN32)
      set(ALLFILES	
         ${ALLFILES}
         server/tools/windows/tools/OperatingSystem.h
         server/tools/windows/tools/OperatingSystem.cpp
      )	

   ELSEIF(APPLE)
      set(ALLFILES	
         ${ALLFILES}
         server/tools/mac/tools/OperatingSystem.h
         server/tools/mac/tools/OperatingSystem.cpp
      )	

   ELSE()
      set(ALLFILES	
         ${ALLFILES}
         server/tools/linux/tools/OperatingSystem.h
         server/tools/linux/tools/OperatingSystem.cpp
      )	
   ENDIF()
   
   ADD_YADOMS_SOURCES(${ALLFILES})
   
   ADD_SOURCES(TestPluginInformation.cpp)
   
ENDIF()
