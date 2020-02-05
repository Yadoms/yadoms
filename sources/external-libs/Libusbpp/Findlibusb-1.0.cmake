
if(LIBUSB_LIBRARIES AND LIBUSB_INCLUDE_DIRS)
  
  # in cache already
  set(LIBUSB_FOUND TRUE)
  
else()

  find_path(LIBUSB_1_INCLUDE_DIR
    NAMES
      libusb.h
    PATHS
      ${LIBUSB_ROOT}/libusb
  )

  find_library(LIBUSB_1_LIBRARY_DEBUG
    NAMES
      usb-1.0
      libusb-1.0
    PATHS
      ${LIBUSB_ROOT}/lib
      ${LIBUSB_ROOT}/Win32/Debug/lib
    NO_DEFAULT_PATH
  )
  find_library(LIBUSB_1_LIBRARY_RELEASE
    NAMES
      usb-1.0
      libusb-1.0
    PATHS
      ${LIBUSB_ROOT}/lib
      ${LIBUSB_ROOT}/Win32/Release/lib
    NO_DEFAULT_PATH
  )

  set(LIBUSB_INCLUDE_DIRS
    ${LIBUSB_1_INCLUDE_DIR}
  )
  set(LIBUSB_LIBRARIES
    debug ${LIBUSB_1_LIBRARY_DEBUG}
    optimized ${LIBUSB_1_LIBRARY_RELEASE}
   )

  if(LIBUSB_INCLUDE_DIRS AND LIBUSB_LIBRARIES)
     set(LIBUSB_FOUND TRUE)
  endif()

endif()
