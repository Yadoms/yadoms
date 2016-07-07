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

      MESSAGES = 0x80
   };

   enum ECommonCommand
   {
      CO_WR_SLEEP = 1,
      CO_WR_RESET = 2,
      CO_RD_VERSION = 3,
      CO_RD_SYS_LOG = 4,
      CO_WR_SYS_LOG = 5,
      CO_WR_BIST = 6,
      CO_WR_IDBASE = 7,
      CO_RD_IDBASE = 8,
      CO_WR_REPEATER = 9,
      CO_RD_REPEATER = 10,
      CO_WR_FILTER_ADD = 11,
      CO_WR_FILTER_DEL = 12,
      CO_WR_FILTER_DEL_ALL = 13,
      CO_WR_FILTER_ENABLE = 14,
      CO_RD_FILTER = 15,
      CO_WR_WAIT_MATURITY = 16,
      CO_WR_SUBTEL = 17,
      CO_WR_MEM = 18,
      CO_RD_MEM = 19,
      CO_RD_MEM_ADDRESS = 20,
      CO_RD_SECURITY = 21,
      CO_WR_SECURITY = 22,
      CO_WR_LEARNMODE = 23,
      CO_RD_LEARNMODE = 24,
      CO_WR_SECUREDEVICE_ADD = 25,
      CO_WR_SECUREDEVICE_DEL = 26,
      CO_RD_SECUREDEVICE_BY_INDEX = 27,
      CO_WR_MODE = 28,
      CO_RD_NUMSECUREDEVICES = 29,
      CO_RD_SECUREDEVICE_BY_ID = 30,
      CO_WR_SECUREDEVICE_ADD_PSK = 31,
      CO_WR_SECUREDEVICE_SENDTEACHIN = 32,
      CO_WR_TEMPORARY_RLC_WINDOW = 33,
      CO_RD_SECUREDEVICE_PSK = 34,
      CO_RD_DUTYCYCLE_LIMIT = 35,
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

} // namespace message
