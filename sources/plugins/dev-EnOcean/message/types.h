#pragma once

namespace message
{   
   // Timeout between 2 bytes when receiving from dongle
   static const auto EnOceanInterByteTimeout = boost::posix_time::milliseconds(100);

   // Timeout when expecting a answer from dongle
   static const auto EnOceanAnswerTimeout(boost::posix_time::seconds(500));

   static const auto SYNC_BYTE_VALUE = 0x55;

   enum
   {
      kOffsetSyncByte = 0,
      kOffsetDataLength = 1,
      kOffsetOptionalLength = 3,
      kOffsetPacketType = 4,
      kOffsetCrc8Header = 5,
      kOffsetData = 6,
   };

   enum EPacketType
   {
      RADIO_ERP1 = 0x01,
      RESPONSE = 0x02,
      RADIO_SUB_TEL = 0x03,
      EVENT = 0x04,
      COMMON_COMMAND = 0x05,
      SMART_ACK_COMMAND = 0x06,
      REMOTE_MAN_COMMAND = 0x07,
      RADIO_MESSAGE = 0x09,
      RADIO_ERP2 = 0x0A,
      RADIO_802_15_4 = 0x10,
      COMMAND_2_4 = 0x11,

      MESSAGES_OFFSET = 0x80
   };

   enum EReturnCode
   {
      RET_OK = 0x00,
      RET_ERROR = 0x01,
      RET_NOT_SUPPORTED = 0x02,
      RET_WRONG_PARAM = 0x03,
      RET_OPERATION_DENIED = 0x04,
      RET_LOCK_SET = 0x05,
      RET_BUFFER_TO_SMALL = 0x06,
      RET_NO_FREE_BUFFER = 0x07,

      RET_SEPCIAL_RC_OFFSET = 0x80,
   };


   // Sizef of messages
   enum { RESPONSE_DONGLE_VERSION_SIZE = 33 };

   // Specific RORG value
   enum { RORG_UNIVERSAL_TEACH_IN = 0xD4 };

} // namespace message
