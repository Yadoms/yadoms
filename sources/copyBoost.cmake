# Macro helper to copy DLL built Boost libraries
#


MACRO(POST_BUILD_COPY_BOOST_LIBRARIES target)

   ##################################################################################################
   ## Post build commands
   ##################################################################################################

   #create lib lists, one for debug libs and another for optimized (release)
   # Boost_LIBRARIES is a list which contains something like this:
   #
   #	debug							(<- define that next list entry if for debug mode)
   #	path/to/debug/boost-...gd.lib)
   #	optimize                        (<- define that next list entry if for release mode)
   #	path/to/debug/boost-...lib)
   #	...
   #
   # In some configurations, the list contains only lib file path without (debug/optimize)
   # So if debug and optimize or not found, then just add library to debug or optimized list
   #
   set(TYPEISKNOWN OFF)
   set(TYPEISDEBUG OFF)
   FOREACH (BOOSTLIBRARY ${Boost_LIBRARIES})
      if(TYPEISKNOWN)
         if(TYPEISDEBUG)
            list(APPEND BOOST_LIBS_DEBUG ${BOOSTLIBRARY})
         else()
            list(APPEND BOOST_LIBS_OPTIMIZED ${BOOSTLIBRARY})
         endif()
         set(TYPEISKNOWN OFF)
      else()
         if(${BOOSTLIBRARY} MATCHES "debug") 
            set(TYPEISKNOWN ON)
            set(TYPEISDEBUG ON)
         elseif(${BOOSTLIBRARY} MATCHES "optimized") 
            set(TYPEISKNOWN ON)
            set(TYPEISDEBUG OFF)
         else()
            list(APPEND BOOST_LIBS_DEBUG ${BOOSTLIBRARY})
         endif()
      endif()
   ENDFOREACH(BOOSTLIBRARY)

   
  
   
   #Take each boost lib file, find its dll and add it to a post build command
   if(NOT Boost_USE_STATIC_LIBS)
      MESSAGE(STATUS "Add boost libraries to be copied as postbuild")
      #Generate post build command for debug boost libs
      FOREACH (BOOSTLIBRARY ${BOOST_LIBS_DEBUG})

         #construct the dll/so fil path
         get_filename_component(UTF_BASE_NAME ${BOOSTLIBRARY} NAME_WE)
         get_filename_component(UTF_PATH ${BOOSTLIBRARY} PATH)
      
         if(WIN32)
            #windows
            set(LIBFILE ${UTF_PATH}/${UTF_BASE_NAME}.dll)
         elseif(APPLE)
            #mac
            set(LIBFILE ${UTF_PATH}/${UTF_BASE_NAME}.dylib)
         else()
            #linux
            set(LIBFILE ${UTF_PATH}/${UTF_BASE_NAME}.so)
         endif()
         
         #check existance
         if(EXISTS ${LIBFILE})
			#add it to found libs
			SET(BOOST_FOUND_DEBUG_BINS ${BOOST_FOUND_DEBUG_BINS} ${LIBFILE})

            #create post build command
            add_custom_command(TARGET ${target} POST_BUILD        	# Adds a post-build event to ${target}
               COMMAND ${CMAKE_COMMAND} -E $<$<CONFIG:debug>:copy_if_different>$<$<NOT:$<CONFIG:debug>>:echo>  	# which executes "cmake - E copy_if_different..."
                  "$<$<CONFIG:debug>:${LIBFILE}>"     							# <--this is in-file
                  $<TARGET_FILE_DIR:${target}>)   				# <--this is out-file path
         else()
            MESSAGE("    Library DEBUG file not found : ${LIBFILE}")
         endif()
      ENDFOREACH(BOOSTLIBRARY)
      
      #Generate post build command for NON debug boost libs
      FOREACH (BOOSTLIBRARY ${BOOST_LIBS_OPTIMIZED})

         #construct the dll/so fil path
         get_filename_component(UTF_BASE_NAME ${BOOSTLIBRARY} NAME_WE)
         get_filename_component(UTF_PATH ${BOOSTLIBRARY} PATH)
      
         if(WIN32)
            #windows
            set(LIBFILE ${UTF_PATH}/${UTF_BASE_NAME}.dll)
         elseif(APPLE)
            #mac
            set(LIBFILE ${UTF_PATH}/${UTF_BASE_NAME}.dylib)
         else()
            #linux
            set(LIBFILE ${UTF_PATH}/${UTF_BASE_NAME}.so)
         endif()
         
         #check existance
         if(EXISTS ${LIBFILE})
			#add it to found libs
			SET(BOOST_FOUND_RELEASE_BINS ${BOOST_FOUND_RELEASE_BINS} ${LIBFILE})
			
            #create post build command
            add_custom_command(TARGET ${target} POST_BUILD        	# Adds a post-build event to ${target}
               COMMAND ${CMAKE_COMMAND} -E $<$<CONFIG:debug>:echo>$<$<NOT:$<CONFIG:debug>>:copy_if_different>  	# which executes "cmake - E copy_if_different..."
                  "$<$<NOT:$<CONFIG:debug>>:${LIBFILE}>"     							# <--this is in-file
                  $<TARGET_FILE_DIR:${target}>)   				# <--this is out-file path
         else()
            MESSAGE("    Library DEBUG file not found : ${LIBFILE}")
         endif()
      ENDFOREACH(BOOSTLIBRARY)
      
      

   endif()		

ENDMACRO()