########################
## Shared Event
########################

add_definitions( -DBOOST_AUTO_TEST_MAIN )

IF (USE_TESTS_SHARED_EVENT)
  set(YADOMS_SRC ../../sources/)
  set(YADOMS_TESTS_SHARED_EVENT
    ${YADOMS_SRC}/shared/shared/Event/Event.hpp
    ${YADOMS_SRC}/shared/shared/Event/EventBase.hpp
    ${YADOMS_SRC}/shared/shared/Event/EventHandler.hpp

    ../mock/shared/stdafx.h
    
    MainTestEvent.cpp
	)
   add_executable(MainTestEvent ${YADOMS_TESTS_SHARED_EVENT} ${YADOMS_SRC})
   set_property( TARGET MainTestEvent PROPERTY INCLUDE_DIRECTORIES ${MOCK_SHARED_INCL_DIR} ${BOOST_INCL_DIR} ${SHARED_INCL_DIR})
	target_link_libraries(MainTestEvent ${LIBS} ${CMAKE_DL_LIBS})
ENDIF()
