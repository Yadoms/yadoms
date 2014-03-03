
IF(NOT DISABLE_TEST_START_OPTIONS_LOADER)
   set(YADOMS_SRC ../../../sources/)
   set(YADOMS_TEST_STARTUP_OPTIONS_LOADER_SRC
      ${YADOMS_SRC}/server/startupOptions/Loader.h
      ${YADOMS_SRC}/server/startupOptions/Loader.cpp
      ${YADOMS_SRC}/server/startupOptions/InvalidOptionException.hpp
      ${YADOMS_SRC}/server/startupOptions/LoaderCustomValidators.hpp
      ${YADOMS_SRC}/server/startupOptions/LoaderException.hpp
      ${YADOMS_SRC}/server/startupOptions/MustExistPathOption.hpp
      ${YADOMS_SRC}/server/startupOptions/IStartupOptions.h
      ${YADOMS_SRC}/server/startupOptions/ValidIpAddressOption.hpp

      mock/stdafx.h

      MainTestStartOptionsLoader.cpp
   )
   add_executable(MainTestStartOptionsLoader ${YADOMS_TEST_STARTUP_OPTIONS_LOADER_SRC})
   set_property( TARGET MainTestStartOptionsLoader PROPERTY INCLUDE_DIRECTORIES ${MOCK_INCL_DIR} ${BOOST_INCL_DIR} ${SHARED_INCL_DIR})
   target_link_libraries(MainTestStartOptionsLoader ${LIBS} ${CMAKE_DL_LIBS})
ENDIF()
