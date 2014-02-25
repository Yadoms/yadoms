########################
## StartupOptions
########################

add_definitions( -DBOOST_AUTO_TEST_MAIN )

IF (USE_TESTS_DIRECTORY_CHANGE_LISTENER)
  set(YADOMS_SRC ../../../../sources/)
  set(USE_TESTS_DIRECTORY_CHANGE_LISTENER_SRC
    ${YADOMS_SRC}/shared/shared/ThreadBase.h
    ${YADOMS_SRC}/shared/shared/ThreadBase.cpp

    ${YADOMS_SRC}/server/tools/tools/DirectoryChangeListener.h
    ${YADOMS_SRC}/server/tools/tools/DirectoryChangeListener.cpp

    ../../../mock/shared/stdafx.h
    ../../../mock/shared/shared/Export.h
    ../../../mock/shared/shared/Log.h
    
    MainTestDirectoryChangeListener.cpp   
  )

  IF(WIN32)
  set(USE_TESTS_DIRECTORY_CHANGE_LISTENER_SRC ${USE_TESTS_DIRECTORY_CHANGE_LISTENER_SRC}
    ${YADOMS_SRC}/server/tools/windows/tools/dir_monitor/basic_dir_monitor_service.hpp
    ${YADOMS_SRC}/server/tools/windows/tools/dir_monitor/dir_monitor_impl.hpp
  )
  ELSEIF(APPLE)
  set(USE_TESTS_DIRECTORY_CHANGE_LISTENER_SRC ${USE_TESTS_DIRECTORY_CHANGE_LISTENER_SRC}
    ${YADOMS_SRC}/server/tools/mac/tools/dir_monitor/basic_dir_monitor_service.hpp
    ${YADOMS_SRC}/server/tools/mac/tools/dir_monitor/dir_monitor_impl.hpp
  )	
  ELSE()
  set(USE_TESTS_DIRECTORY_CHANGE_LISTENER_SRC ${USE_TESTS_DIRECTORY_CHANGE_LISTENER_SRC}
    ${YADOMS_SRC}/server/tools/linux/tools/dir_monitor/basic_dir_monitor_service.hpp
    ${YADOMS_SRC}/server/tools/linux/tools/dir_monitor/dir_monitor_impl.hpp
  )	
  ENDIF()

  add_executable(MainTestDirectoryChangeListener ${USE_TESTS_DIRECTORY_CHANGE_LISTENER_SRC})
  set_property( TARGET MainTestDirectoryChangeListener PROPERTY INCLUDE_DIRECTORIES ${MOCK_SHARED_INCL_DIR} ${BOOST_INCL_DIR} ${TOOLS_PF_INCL_DIR} ${SHARED_INCL_DIR})
  target_link_libraries(MainTestDirectoryChangeListener ${LIBS} ${CMAKE_DL_LIBS})
ENDIF()