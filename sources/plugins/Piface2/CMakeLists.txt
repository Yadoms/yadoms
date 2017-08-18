# include common plugin definitions

if(CMAKE_YADOMS_PLATFORM STREQUAL "Raspberry")

   include("../YadomsPluginsCommon.cmake")

   #########################################
   ## Place here all the plugin source files
   #########################################
   set(PIFACE2_PLUGIN_SOURCES
      stdafx.h
      stdafx.cpp
      eventDefinitions.h
      IIO.h
      InitializationException.hpp
      IO.h
      IO.cpp
      IOManager.h
      IOManager.cpp
      IPf2Configuration.h
      Pf2Configuration.h
      Pf2Configuration.cpp
      Piface2.h
      Piface2.cpp
      Piface2Factory.h
      Piface2Factory.cpp
      SPIException.hpp
      )

   ADD_SUBDIRECTORY(libmcp23s17)  
   ADD_SUBDIRECTORY(libpifacedigital)
    
   #########################################
   ## Place here all additional include directories
   ## (by default yadoms-shared and boost are included)
   #########################################

   set(PIFACE2_PLUGIN_ADDITIONAL_INC_DIR
       ${PIFACE2DIGITAL_INCLUDE_DIRECTORY}
       ${MCP23S17_INCLUDE_DIRECTORY})

   set(PIFACE2_PLUGIN_INCLDIR  
       ${CMAKE_CURRENT_SOURCE_DIR})

   #########################################
   ## Place here all additional libraries to link with
   ## (by default yadoms-shared and boost are included)
   #########################################

   set(PIFACE2_PLUGIN_LINK
      mcp23s17
      pifacedigital
   )

   #########################################
   ## Place here all additional compiler flags
   ## i.e. : for raspberry to link with wiringPi : use add_definition("-lwiringPi")
   #########################################

   #########################################
   ## Always leave these lines to configure plugin correctly
   ## even if FAKE_PLUGIN_INCLDIR or FAKE_PLUGIN_LINK are empty
   #########################################

   PLUGIN_SOURCES(Piface2 ${PIFACE2_PLUGIN_SOURCES})
   PLUGIN_INCLDIR(Piface2 ${PIFACE2_PLUGIN_INCLDIR} ${PIFACE2_PLUGIN_ADDITIONAL_INC_DIR})
   PLUGIN_LINK   (Piface2 ${PIFACE2_PLUGIN_LINK})

   #########################################
   ## Post-build commands
   ## Copy files to build directory
   ## "package.json" and "icon.png" are needed, but you can add extra-files
   #########################################
   PLUGIN_POST_BUILD_COPY_FILE(Piface2 package.json)
   PLUGIN_POST_BUILD_COPY_FILE(Piface2 icon.png)
   PLUGIN_POST_BUILD_COPY_DIRECTORY(Piface2 locales)

endif()
