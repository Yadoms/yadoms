set(SHARED_FILES_FOR_PLUGINS
../tools/ThreadBase.cpp
../tools/ThreadBase.h)

# define all plugins library
add_library(fakePlugin MODULE  hardware/fakePlugin.cpp ${SHARED_FILES_FOR_PLUGINS})

target_link_libraries(fakePlugin ${LIBS} ${CMAKE_DL_LIBS})