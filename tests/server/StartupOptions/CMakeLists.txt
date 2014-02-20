# include common plugin definitions
#include("../YadomsHPluginsCommon.cmake")
#include("../../YadomsTestsCommon.cmake")

########################
## StartupOptions
########################

add_definitions( -DBOOST_AUTO_TEST_MAIN )

IF (USE_TESTS_START_OPTIONS_LOADER)
   set(YADOMS_TESTS_STARTUP_OPTIONS_LOADER_SRC
#       ${YADOMS_SRC} #FIXME : Pas moyen d'utiliser directement cela au lieu des autres chemins dessous ???

   ../../../sources/server/stdafx.cpp
   ../../../sources/server/stdafx.h
  
	../../../sources/server/StartupOptions/Loader.h
	../../../sources/server/StartupOptions/Loader.cpp
	../../../sources/server/StartupOptions/InvalidOptionException.hpp
	../../../sources/server/StartupOptions/LoaderCustomValidators.hpp
	../../../sources/server/StartupOptions/LoaderException.hpp
	../../../sources/server/StartupOptions/MustExistPathOption.hpp
	../../../sources/server/StartupOptions/StartupOptions.h
	../../../sources/server/StartupOptions/ValidIpAddressOption.hpp

	MainTestStartOptionsLoader.cpp
      )

   add_executable(MainTestStartOptionsLoader ${YADOMS_TESTS_STARTUP_OPTIONS_LOADER_SRC} ${YADOMS_SRC})
   add_precompiled_header(MainTestStartOptionsLoader stdafx.h)
   set_property( TARGET MainTestStartOptionsLoader 
                 PROPERTY INCLUDE_DIRECTORIES ${YADOMS_INCL_DIR} ${SHARED_INCL_DIR} ${BOOST_INCL_DIR}
               )
   target_link_libraries(MainTestStartOptionsLoader ${LIBS} ${CMAKE_DL_LIBS} )

#	GENERATE_TEST_EXECUTABLE ( MainTestStartOptionsLoader, ${YADOMS_TESTS_STARTUP_OPTIONS_LOADER_SRC})

ENDIF()
