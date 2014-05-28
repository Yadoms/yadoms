
IF(NOT DISABLE_TEST_SHARED_EVENT)
   ADD_YADOMS_SOURCES(
      shared/shared/event/Event.hpp
      shared/shared/event/EventBase.hpp
      shared/shared/event/EventHandler.hpp
      shared/shared/event/EventTimer.h
      shared/shared/event/EventTimer.cpp
      shared/shared/event/EventTimePoint.h
      shared/shared/event/EventTimePoint.cpp
   	shared/shared/event/Now.h
	   shared/shared/event/Now.cpp)
   
   ADD_SOURCES(
      TestEvent.cpp
      TestTimePoint.cpp
      TestEventTimer.cpp)
   
ENDIF()
