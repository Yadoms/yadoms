
IF(NOT DISABLE_TEST_JOB)
   ADD_YADOMS_SOURCES(
      shared/shared/ThreadBase.h
      shared/shared/ThreadBase.cpp
      server/job/INotificationObserverForJobsManager.h
      server/job/NotificationObserverForJobsManager.h
      server/job/NotificationObserverForJobsManager.cpp)
   
   ADD_SOURCES(TestNotificationObserverForJobsManager.cpp)
   
ENDIF()
