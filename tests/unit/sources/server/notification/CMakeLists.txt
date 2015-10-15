
IF(NOT DISABLE_TEST_SHARED_NOTIFICATION)
   ADD_YADOMS_SOURCES(
      server/notification/NotificationCenter.h
      server/notification/NotificationCenter.cpp
      server/notification/change/Type.h
      server/notification/change/Type.cpp
      shared/shared/event/Event.hpp
      shared/shared/event/EventBase.hpp
      shared/shared/event/EventHandler.hpp
      shared/shared/event/EventTimer.h
      shared/shared/event/EventTimer.cpp
      shared/shared/event/EventTimePoint.h
      shared/shared/event/EventTimePoint.cpp	
	)
   
   ADD_SOURCES(
      TestNotification.cpp
	)
   
ENDIF()
