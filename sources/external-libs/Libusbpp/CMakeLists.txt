
set(LIBUSBPP_DIR_NAME Libusbpp-master-2017-10-17)

set(LIBUSBPP_SOURCES
   ${LIBUSBPP_DIR_NAME}/src/Configuration.cpp
   ${LIBUSBPP_DIR_NAME}/src/ConfigurationImpl.cpp
   ${LIBUSBPP_DIR_NAME}/src/ConfigurationImpl.hpp
   ${LIBUSBPP_DIR_NAME}/src/Device.cpp
   ${LIBUSBPP_DIR_NAME}/src/DeviceImpl.cpp
   ${LIBUSBPP_DIR_NAME}/src/DeviceImpl.hpp
   ${LIBUSBPP_DIR_NAME}/src/Endpoint.cpp
   ${LIBUSBPP_DIR_NAME}/src/EndpointImpl.cpp
   ${LIBUSBPP_DIR_NAME}/src/EndpointImpl.hpp
   ${LIBUSBPP_DIR_NAME}/src/Exception.cpp
   ${LIBUSBPP_DIR_NAME}/src/Interface.cpp
   ${LIBUSBPP_DIR_NAME}/src/InterfaceImpl.cpp
   ${LIBUSBPP_DIR_NAME}/src/InterfaceImpl.hpp
   ${LIBUSBPP_DIR_NAME}/src/LibusbImpl.cpp
   ${LIBUSBPP_DIR_NAME}/src/LibusbImpl.hpp
   ${LIBUSBPP_DIR_NAME}/src/libusbpp.cpp
   ${LIBUSBPP_DIR_NAME}/src/Transfer.cpp
   ${LIBUSBPP_DIR_NAME}/src/TransferImpl.cpp
   ${LIBUSBPP_DIR_NAME}/src/TransferImpl.hpp
   ${LIBUSBPP_DIR_NAME}/src/Wideconvert.cpp
   ${LIBUSBPP_DIR_NAME}/src/Wideconvert.hpp
)

include (Findlibusb-1.0.cmake)

if(NOT ${LIBUSB_1_FOUND})
   message(FATAL_ERROR "libusb-1.0 required but not found")
endif()

set(LIBUSBPP_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${LIBUSBPP_DIR_NAME}/headers)

include_directories(
   ${LIBUSB_1_INCLUDE_DIR}
   ${LIBUSBPP_INCLUDE_DIRECTORY}
)

add_library(libusbpp STATIC ${LIBUSBPP_SOURCES})

#target_link_libraries(libusbpp ${LIBUSB_1_LIBRARIES})


SET_PROPERTY(TARGET libusbpp PROPERTY FOLDER "External Libs")

#TODO nécessaire ?
#install(FILES ${LIBUSB_1_LIBRARIES}
#   DESTINATION ${INSTALL_BINDIR})

set(LIBUSBPP_INCLUDE_DIRS ${LIBUSBPP_INCLUDE_DIRECTORY} PARENT_SCOPE)
set(LIBUSBPP_LIBRARIES libusbpp ${LIBUSB_1_LIBRARIES} PARENT_SCOPE)
set(LIBUSBPP_FOUND TRUE PARENT_SCOPE)
