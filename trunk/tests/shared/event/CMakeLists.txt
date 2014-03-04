
IF (NOT DISABLE_TEST_SHARED_EVENT)
   set(YADOMS_SRC ../../../sources/)
   set(YADOMS_TEST_SHARED_EVENT
      ${YADOMS_SRC}/shared/shared/event/Event.hpp
      ${YADOMS_SRC}/shared/shared/event/EventBase.hpp
      ${YADOMS_SRC}/shared/shared/event/EventHandler.hpp

      ../../mock/shared/stdafx.h

      MainTestEvent.cpp
   )
   add_executable(MainTestEvent ${YADOMS_TEST_SHARED_EVENT} ${YADOMS_SRC})
   set_property( TARGET MainTestEvent PROPERTY INCLUDE_DIRECTORIES ${MOCK_SHARED_INCL_DIR} ${BOOST_INCL_DIR} ${SHARED_INCL_DIR})
   target_link_libraries(MainTestEvent ${LIBS} ${CMAKE_DL_LIBS})
   COPY_BOOST_LIBS(MainTestEvent)
ENDIF()
