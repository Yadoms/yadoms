
IF(NOT DISABLE_TEST_AUTOMATION)
   ADD_YADOMS_SOURCES(
      shared/shared/ThreadBase.h
      shared/shared/ThreadBase.cpp
      server/automation/INotificationObserverForRulesManager.h
      server/automation/NotificationObserverForRulesManager.h
      server/automation/NotificationObserverForRulesManager.cpp)
   
   ADD_SOURCES(TestNotificationObserverForRulesManager.cpp)
   
ENDIF()

