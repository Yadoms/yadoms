
IF(NOT DISABLE_TEST_RFXCOM)
   ADD_YADOMS_SOURCES(
      plugins/Rfxcom/rfxcomMessages/RareDeviceIdFilter.cpp
      plugins/Rfxcom/PairingHelper.h
      plugins/Rfxcom/PairingHelper.cpp
      plugins/Rfxcom/PicBootReceiveBufferHandler.h
      plugins/Rfxcom/PicBootReceiveBufferHandler.cpp
      plugins/Rfxcom/RfxComReceiveBufferHandler.h
      plugins/Rfxcom/RfxComReceiveBufferHandler.cpp
      shared/shared/communication/NoBufferLogger.h
      shared/shared/communication/NoBufferLogger.cpp
   )
   
   ADD_SOURCES(
      TestIncrementSequenceNumber.cpp
      TestRareDeviceIdFilter.cpp
      TestRfxComReceiveBufferHandler.cpp
      TestPairingHelper.cpp
      TestPicBootReceiveBufferHandler.cpp
   )
   
ENDIF()
