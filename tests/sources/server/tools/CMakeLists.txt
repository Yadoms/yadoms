
IF(NOT DISABLE_TEST_DIRECTORY_CHANGE_LISTENER)
   ADD_YADOMS_SOURCES(
      server/tools/tools/DirectoryChangeListener.h
      server/tools/tools/DirectoryChangeListener.cpp
      shared/shared/ThreadBase.h
      shared/shared/ThreadBase.cpp)
   
   ADD_SOURCES(TestDirectoryChangeListener.cpp)
   
   IF(WIN32)
      ADD_YADOMS_SOURCES(
         server/tools/windows/tools/dir_monitor/basic_dir_monitor_service.hpp
         server/tools/windows/tools/dir_monitor/dir_monitor_impl.hpp)
   ELSEIF(APPLE)
      ADD_YADOMS_SOURCES(
         server/tools/mac/tools/dir_monitor/basic_dir_monitor_service.hpp
         server/tools/mac/tools/dir_monitor/dir_monitor_impl.hpp)
   ELSE()
      ADD_YADOMS_SOURCES(
         server/tools/linux/tools/dir_monitor/basic_dir_monitor_service.hpp
         server/tools/linux/tools/dir_monitor/dir_monitor_impl.hpp)
   ENDIF()
   
ENDIF()
