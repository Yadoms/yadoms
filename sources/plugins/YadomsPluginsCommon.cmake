# Macros for setting up a plugin

# Use a specific package.json file
# If used, plugin must do its post-build copy
# Note that ${CMAKE_CURRENT_BINARY_DIR}/package.json remains generated from
# package.in.json to include plugin version number at makefile generation time.
# Plugin can work with this file to produced a specific package.json file.
MACRO(USE_SPECIFIC_PACKAGE_JSON _packageJsonFile)
   set(SPECIFIC_PACKAGE_JSON ${_packageJsonFile})
ENDMACRO()

# Use a specific locales directory
# If used, plugin must do its post-build copy
MACRO(USE_SPECIFIC_LOCALES_DIRECTORY _localesDirectory)
   set(SPECIFIC_LOCALES_DIRECTORY ${_localesDirectory})
ENDMACRO()

MACRO(MAKE_PACKAGE_JSON _targetName)
   # Workaround to force CMake to rebuild makefile when changelog.md is updated
   configure_file(changelog.md
                  ${CMAKE_CURRENT_BINARY_DIR}/changelog.md.dummy)
   
   configure_file(package.in.json
                  ${CMAKE_CURRENT_BINARY_DIR}/package.json
                  @ONLY
                  NEWLINE_STYLE UNIX)

ENDMACRO()

function(PLUGIN_IS_IN_DEV_STATE _targetName)
   string(FIND ${_targetName} "dev-" DEV_SUBSTRING_POSITION)
   if (${DEV_SUBSTRING_POSITION} EQUAL 0)
      set(DEV_PLUGIN TRUE PARENT_SCOPE)
   else()
      set(DEV_PLUGIN FALSE PARENT_SCOPE)
   endif()
endfunction()

MACRO(PLUGIN_SOURCES _targetName)
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${YADOMS_OUTPUT_DIR}/plugins/${_targetName} )
   foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
       string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
       set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${YADOMS_OUTPUT_DIR}/${OUTPUTCONFIG}/plugins/${_targetName})
   endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
   
   # Retrieve plugin version
   GET_PACKAGE_VERSION("changelog.md" PLUGIN_VERSION)
   EXTRACT_VERSION(${PLUGIN_VERSION} PLUGIN_VERSION_MAJOR PLUGIN_VERSION_MINOR PLUGIN_VERSION_PATCH)
   
   FILE(GLOB TRANSLATION_FILES locales/*)
   source_group(locales locales/*)
   
   set(PLUGIN_SOURCE_FILES
      ${ARGN}
      package.in.json
      changelog.md
      icon.png
      ${TRANSLATION_FILES}
      )

   ##################################################################################################
   ## Resources
   ##################################################################################################
   if(WIN32)

      ##################################################################################################
      ## Pre build commands (only visual studio)
      ##################################################################################################
      if(MSVC)
         set(PLUGIN_NAME ${_targetName})

         #Try to use plugin icon
         FILE(GLOB PLUGIN_EXE_ICON icon.ico)
         if(NOT EXISTS ${PLUGIN_EXE_ICON})
            SET(PLUGIN_EXE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/../common/resources/windows/plugin.ico")
         endif(NOT EXISTS ${PLUGIN_EXE_ICON})         
         
         #update the Windows specific 'plugin.rc' file which add properties to executable (version, releaseType)
         #as pre build step (plugin.rc is modified only if needed, to avoid unjustified build
         # apply templating to the manifest for setting the version
         # PLUGIN_EXE_ICON is used in "plugin.rc.in"
         configure_file(../common/resources/windows/plugin.rc.in
            "${CMAKE_BINARY_DIR}/plugin-${_targetName}-generated.rc"
         @ONLY)
            
         # If we build for windows systems, we also include the resource file
         # containing the manifest, icon and other resources
         set(PLUGIN_SOURCE_FILES ${PLUGIN_SOURCE_FILES} 
            ${PLUGIN_EXE_ICON}
            ${CMAKE_BINARY_DIR}/plugin-${_targetName}-generated.rc
            ../common/resources/windows/resource.h
         )
         source_group(resources ../common/resources/windows/*.*)
      endif(MSVC)
   endif(WIN32)   
   
   
   add_executable(${_targetName} ${PLUGIN_SOURCE_FILES})
   project(${_targetName})
	
   # Build package.json (add version to package.in.json from changelog.md)
   MAKE_PACKAGE_JSON(${_targetName})
	
	IF(MSVC OR XCODE)
		SET_PROPERTY(TARGET ${_targetName} PROPERTY FOLDER "plugins")
	ENDIF()
ENDMACRO()

MACRO(PLUGIN_INCLDIR _targetName)

   #define the list of all include dirs
   set(PLUGINS_ALL_INCLUDE_DIRS
      ${SHARED_INCL_DIR}
      ${BOOST_INCL_DIR}
      ${Poco_INCLUDE_DIRS}
      ${plugin_cpp_api_INCLUDE_DIR}
      ${ARGN}
      )
         
   #in case of OpenSSL found, just add openssl include dir
   if(${OPENSSL_FOUND})
      set(PLUGINS_ALL_INCLUDE_DIRS  ${PLUGINS_ALL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})
   endif()

	set_property( TARGET ${_targetName} PROPERTY INCLUDE_DIRECTORIES ${PLUGINS_ALL_INCLUDE_DIRS})
ENDMACRO()

MACRO(PLUGIN_LINK _targetName)
	target_link_libraries(${_targetName}
      yadoms-shared
      plugin_cpp_api
      ${LIBS}
      ${CMAKE_DL_LIBS}
      ${PROTOBUF_LIBRARIES}
      ${ARGN}
      )
	
   ##################################################################################################
   ## Add natvis (allow better debugging within VisualStudio)
   ##################################################################################################
   ADD_VS_NATVIS(${_targetName})
   
   string(REPLACE "-" "_" ComponentCompatibleName ${_targetName})
   
   #configure plugin as installable component if not in devlopment state (target name begin by 'dev-')
   PLUGIN_IS_IN_DEV_STATE(${_targetName})
   if (NOT ${DEV_PLUGIN})
      install(TARGETS ${_targetName} 
         RUNTIME DESTINATION ${INSTALL_BINDIR}/plugins/${_targetName}
         COMPONENT  ${ComponentCompatibleName})
   else()
      message (STATUS "  (${_targetName} is a dev plugin, it won't be installed)")
   endif()
   ##################################################################################################
   ## RPATH
   ##################################################################################################
   if(CMAKE_CROSSCOMPILING)
	  #Fix RPATH for cross compilation
      set_target_properties(${_targetName} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
   endif(CMAKE_CROSSCOMPILING)
      
   set(PLUGINLIST
      ${PLUGINLIST}
      ${_targetName}
      PARENT_SCOPE)
     

	if(COTIRE_USE)
		set_target_properties(${_targetName} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "stdafx.h")
		
		if(COTIRE_USE_UNITY)
			set_target_properties(${_targetName} PROPERTIES COTIRE_ADD_UNITY_BUILD TRUE)
		else()
			set_target_properties(${_targetName} PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
		endif()
		
		cotire(${_targetName})
		
		if(COTIRE_USE_UNITY)
		   if(CMAKE_CROSSCOMPILING)
		      #Fix RPATH for cross compilation
		      set_target_properties(${_targetName}_unity PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
            ADD_VS_NATVIS(${_targetName}_unity)
		   endif(CMAKE_CROSSCOMPILING)
		endif()
	endif()	

   ##################################################################################################
   ## Manifest
   ##################################################################################################
   if(WIN32)
      if(MSVC)  
         set(PLUGIN_NAME ${_targetName})
         string(TOLOWER ${PLUGIN_NAME} PLUGIN_NAME_LOWER)
         
         #prepare version to be used in windows manifest (only 3 digits)
         set(PLUGIN_VERSION_FOR_MANIFEST "${PLUGIN_VERSION_MAJOR}.${PLUGIN_VERSION_MINOR}.${PLUGIN_VERSION_PATCH}")

         # apply templating to the manifest for setting the version
         configure_file(../common/resources/windows/plugin.exe.manifest.in
            "${CMAKE_BINARY_DIR}/${_targetName}.exe.manifest"
         @ONLY)
      
         #add a custom command
         add_custom_command(TARGET ${_targetName} POST_BUILD
             COMMAND "mt.exe" -manifest \"${CMAKE_BINARY_DIR}\\${_targetName}.exe.manifest\" -outputresource:"$(TargetDir)$(TargetFileName)"\;\#1
             COMMENT "Adding manifest..." 
         )
            
         if(COTIRE_USE)   
            if(COTIRE_USE_UNITY)
               add_custom_command(TARGET ${_targetName}_unity POST_BUILD
                   COMMAND "mt.exe" -manifest \"${CMAKE_BINARY_DIR}\\${_targetName}.exe.manifest\" -outputresource:"$(TargetDir)$(TargetFileName)"\;\#1
                   COMMENT "Adding manifest..." 
               )
            endif(COTIRE_USE_UNITY)
         endif(COTIRE_USE)
      endif(MSVC)
   endif(WIN32)   
	
   # Post-build copy of required files
   if(DEFINED SPECIFIC_PACKAGE_JSON)
      PLUGIN_POST_BUILD_COPY_FILE(${_targetName} ${SPECIFIC_PACKAGE_JSON})
   else()
      PLUGIN_POST_BUILD_COPY_FILE(${_targetName} ${CMAKE_CURRENT_BINARY_DIR}/package.json)
   endif()
   
   PLUGIN_POST_BUILD_COPY_FILE(${_targetName} changelog.md)
   PLUGIN_POST_BUILD_COPY_FILE(${_targetName} icon.png)
   
   if(DEFINED SPECIFIC_LOCALES_DIRECTORY)
      PLUGIN_POST_BUILD_COPY_DIRECTORY(${_targetName} ${SPECIFIC_LOCALES_DIRECTORY}) 
   else()
      PLUGIN_POST_BUILD_COPY_DIRECTORY(${_targetName} locales) 
   endif()
	
ENDMACRO()

# brief Copy a file to the target output dir
# param [in] _targetName The current target (ie: pluginName)
# param [in] _resource The resource (absolute path) to copy to the target output dir
MACRO(PLUGIN_POST_BUILD_COPY_FILE _targetName _resource)

   if (IS_ABSOLUTE ${_resource})
      set(resource ${_resource})
   else()
      set(resource "${CMAKE_CURRENT_SOURCE_DIR}/${_resource}")
   endif()

   get_filename_component(_resourcePath ${resource}  DIRECTORY)
   get_filename_component(_resourceFile ${resource}  NAME)

   string(REPLACE "-" "_" ComponentCompatibleName ${_targetName})

   PLUGIN_IS_IN_DEV_STATE(${_targetName})
   if (NOT ${DEV_PLUGIN})
      install(FILES ${resource} 
            DESTINATION ${INSTALL_BINDIR}/plugins/${_targetName}
            COMPONENT  ${ComponentCompatibleName})
   endif()
         
   add_custom_command(TARGET ${_targetName} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${resource} $<TARGET_FILE_DIR:${_targetName}>/${_resourceFile})
   if(COTIRE_USE)
      if(COTIRE_USE_UNITY)
         add_custom_command(TARGET ${_targetName}_unity POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${resource} $<TARGET_FILE_DIR:${_targetName}_unity>/${_resourceFile})
	  endif()	
   endif()	
ENDMACRO()

# brief Copy a directory (and its content) to the target output dir
# param [in] _targetName The current target (ie: pluginName)
# param [in] _resource The resource folder (absolute path) to copy to the target output dir
# param [in/optional] To specify the target destination (if omit, destination will be the last part of _resource tree)
MACRO(PLUGIN_POST_BUILD_COPY_DIRECTORY _targetName _resource)

   set (extra_args ${ARGN})
   list(LENGTH extra_args num_extra_args)
   if (${num_extra_args} GREATER 0)
      # A target folder is given
      list(GET extra_args 0 where)
   else()
      # No target folder
      if (IS_ABSOLUTE ${_resource})
         get_filename_component(where "${_resource}" NAME)
      else()
         set(where ${_resource})
      endif()
   endif()
   
   if (IS_ABSOLUTE ${_resource})
      set(resource ${_resource})
   else()
      set(resource "${CMAKE_CURRENT_SOURCE_DIR}/${_resource}")
   endif()

   string(REPLACE "-" "_" ComponentCompatibleName ${_targetName})

   PLUGIN_IS_IN_DEV_STATE(${_targetName})
   if (NOT ${DEV_PLUGIN})
      install(DIRECTORY ${resource}
            DESTINATION ${INSTALL_BINDIR}/plugins/${_targetName}/
            COMPONENT  ${ComponentCompatibleName})
   endif()

   add_custom_command(TARGET ${_targetName} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${resource} $<TARGET_FILE_DIR:${_targetName}>/${where})
	  
   if(COTIRE_USE)
      if(COTIRE_USE_UNITY)
         add_custom_command(TARGET ${_targetName}_unity POST_BUILD
           COMMAND ${CMAKE_COMMAND} -E copy_directory ${resource} $<TARGET_FILE_DIR:${_targetName}_unity>/${where})
	  endif()	
   endif()		  
ENDMACRO()


