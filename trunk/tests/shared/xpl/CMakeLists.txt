
IF (NOT DISABLE_TEST_XPL_HELPER)
   set(YADOMS_SRC ../../../sources/)
   set(YADOMS_TEST_XPL_HELPER_SRC
      ${YADOMS_SRC}/shared/shared/xpl/XplHelper.h
      ${YADOMS_SRC}/shared/shared/xpl/XplHelper.cpp

      ../../mock/shared/stdafx.h
      ../../mock/shared/shared/NetworkHelper.h

      MainTestXplHelper.cpp
   )

   add_executable(MainTestXplHelper ${YADOMS_TESTS_XPL_HELPER} ${YADOMS_TEST_XPL_HELPER_SRC})
   set_property( TARGET MainTestXplHelper PROPERTY INCLUDE_DIRECTORIES ${MOCK_SHARED_INCL_DIR} ${BOOST_INCL_DIR} ${SHARED_INCL_DIR})
   target_link_libraries(MainTestXplHelper ${LIBS} ${CMAKE_DL_LIBS})
   COPY_BOOST_LIBS(MainTestXplHelper)
ENDIF()
