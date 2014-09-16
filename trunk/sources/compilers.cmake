# Compilers/IDE specific configurations
#
message("System Name = ${CMAKE_SYSTEM_NAME}")
message("Yadoms Platform = ${CMAKE_YADOMS_PLATFORM}")

if(MSVC)

	#allow solution folders
	SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

	#global disable (for all targets)
	#disable warning C4996   Function call with parameters that may be unsafe 
	add_definitions( "/wd4996 /wd4251" )

	#######################################################
	## Activate warning level4, and disable useless ones ##
	#######################################################
	add_definitions("/W4")
	
	# A lot of level 4 warnings are involved by boost.
	# Use the next link to see how to remove warning 
	#https://svn.boost.org/trac/boost/wiki/Guidelines/WarningsGuidelines
	
	#disable warning C4512: assignment operator could not be generated
	add_definitions( "/wd4512" )
	
	#disable warning C4610: object 'class' can never be instantiated - user-defined constructor required : solution : same as C4512
	add_definitions( "/wd4610" )
	
	#disable warning C4127: conditional expression is constant
	add_definitions( "/wd4127" )

	#disable warning C4100: unreferenced formal parameter
	add_definitions( "/wd4100" )
	
	#disable warning C4714: function 'function' marked as __forceinline not inlined
	add_definitions( "/wd4714" )
	
	#disable warning C4510: 'class' : default constructor could not be generated
	add_definitions( "/wd4510" )

	#disable warning C4505: 'function' : unreferenced local function has been removed
	add_definitions( "/wd4505" )
	
	#disable warning C4503: 'identifier' : decorated name length exceeded, name was truncated
	add_definitions( "/wd4503" )
	
	#to allow BOOST in precompiled header (specify Precompiled Header Memory Allocation Limit) 
	add_definitions("/Zm256")

	#add WIN32_WINNT preprocessor definition
	get_WIN32_WINNT(CURRENT_WINNT_VERSION)
	add_definitions(-D_WIN32_WINNT=${CURRENT_WINNT_VERSION})
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	add_definitions("-Wall")
	add_definitions("-fpermissive")
	add_definitions("-std=c++0x")

	#warning: comparison of unsigned expression < 0 is always false (from boost)
	add_definitions("-Wno-type-limits")

	#warning: unused parameter (from boost)
	add_definitions("-Wno-unused-parameter")
	
	#warning:  typedef �value_t� locally defined but not used [-Wunused-local-typedefs]
	add_definitions("-Wno-unused-local-typedefs")
	
endif()


if (CMAKE_COMPILER_IS_CLANG)
	add_definitions("-Wall -Wextra -pedantic")
	add_definitions("-std=c++0x")
   add_definitions("-Woverloaded-virtual")
   add_definitions("-Wunused-variable")
endif()

#Cross compiling for Raspberry
if (CMAKE_COMPILER_IS_RASPBERRY_CROSS_COMPILER)
	message("Cross compile for Raspberry")   
	add_definitions("-Wno-psabi")
endif()

# with -fPIC
IF(UNIX AND NOT WIN32)
   FIND_PROGRAM(CMAKE_UNAME uname /bin /usr/bin /usr/local/bin )
   IF(CMAKE_UNAME)
     EXEC_PROGRAM(uname ARGS -m OUTPUT_VARIABLE CMAKE_SYSTEM_PROCESSOR)
     SET(CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR} CACHE INTERNAL "processor type (i386 and x86_64)")
     IF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
       ADD_DEFINITIONS(-fPIC)
     ENDIF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
   ENDIF(CMAKE_UNAME)
ENDIF(UNIX AND NOT WIN32)

#check eclispe debug options
if(DEBUG_WITH_ECLIPSE)
	add_definitions(-DDEBUG)
	add_definitions("-g")
endif()
