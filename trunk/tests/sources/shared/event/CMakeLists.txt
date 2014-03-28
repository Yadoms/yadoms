
IF(NOT DISABLE_TEST_SHARED_EVENT)
   ADD_YADOMS_SOURCES(
      shared/shared/event/Event.hpp
      shared/shared/event/EventBase.hpp
      shared/shared/event/EventHandler.hpp)
   
   ADD_SOURCES(TestEvent.cpp)
   
ENDIF()
