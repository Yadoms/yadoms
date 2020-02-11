
# First find libusb (dependency of libusbpp)
include (Findlibusb-1.0.cmake)

if(${LIBUSB_FOUND})

   # Build libsusbpp
   
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

   set(LIBUSBPP_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${LIBUSBPP_DIR_NAME}/headers)

   include_directories(
      ${LIBUSB_INCLUDE_DIRS}
      ${LIBUSBPP_INCLUDE_DIRECTORY}
   )

   add_library(libusbpp STATIC ${LIBUSBPP_SOURCES})

   SET_PROPERTY(TARGET libusbpp PROPERTY FOLDER "External Libs")

   set(LIBUSBPP_INCLUDE_DIRS ${LIBUSBPP_INCLUDE_DIRECTORY} PARENT_SCOPE)
   set(LIBUSBPP_LIBRARIES libusbpp ${LIBUSB_LIBRARIES} PARENT_SCOPE)
   set(LIBUSBPP_FOUND TRUE PARENT_SCOPE)

   message(STATUS "Found libusb-1.0")
   
else()

   message(WARNING "libusb-1.0 not found. Plugins using USB won't be built.")
   set(LIBUSBPP_FOUND FALSE PARENT_SCOPE)
   
endif()

