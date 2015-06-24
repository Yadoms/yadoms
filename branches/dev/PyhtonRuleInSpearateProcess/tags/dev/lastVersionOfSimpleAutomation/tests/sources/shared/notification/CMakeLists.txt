
IF(NOT DISABLE_TEST_SHARED_NOTIFICATION)
   ADD_YADOMS_SOURCES(
      shared/shared/notification/NotificationObserver.h
      shared/shared/notification/NotificationObserver.cpp
      shared/shared/notification/NotificationCenter.h
      shared/shared/notification/NotificationCenter.cpp
      shared/shared/event/Event.hpp
      shared/shared/event/EventBase.hpp
      shared/shared/event/EventHandler.hpp
      shared/shared/event/EventTimer.h
      shared/shared/event/EventTimer.cpp
      shared/shared/event/EventTimePoint.h
      shared/shared/event/EventTimePoint.cpp
   	shared/shared/event/Now.h
	   shared/shared/event/Now.cpp		
	)
   
   ADD_SOURCES(
      TestNotification.cpp
	)
   
ENDIF()
