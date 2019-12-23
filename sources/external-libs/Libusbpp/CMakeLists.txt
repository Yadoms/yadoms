#
# Required CMake modules
include(CheckIncludeFile)#TODO conserver ?
include(CheckFunctionExists)#TODO conserver ?


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

#TODO il existe un FindLibUSB-1.0.cmake dans ${LIBUSBPP_DIR_NAME}/cmake ==> A utiliser !
set(LIBUSB1_INCLUDE_DIR ${LIBUSB_ROOT}/include/libusb-1.0)
set(LIBUSB1_LIBRARIES ${LIBUSB_ROOT}/MS32/dll/libusb-1.0.lib)#TODO pas bon, chemin vers la lib windows en dur

set(LIBUSBPP_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${LIBUSBPP_DIR_NAME}/headers)

include_directories(
   ${LIBUSB1_INCLUDE_DIR}
   ${LIBUSBPP_INCLUDE_DIRECTORY}
)

add_library(libusbpp STATIC ${LIBUSBPP_SOURCES})

#TODO virer target_link_libraries(libusbpp "${LIBUSB1_LIBRARIES}" )


IF(WIN32)
   SET_PROPERTY(TARGET libusbpp PROPERTY FOLDER "External Libs")
ENDIF()


set(LIBUSBPP_INCLUDE_DIRECTORIES ${LIBUSB1_INCLUDE_DIR} ${LIBUSBPP_INCLUDE_DIRECTORY} PARENT_SCOPE)
set(LIBUSBPP_LIBRARIES libusbpp ${LIBUSB1_LIBRARIES} PARENT_SCOPE)
