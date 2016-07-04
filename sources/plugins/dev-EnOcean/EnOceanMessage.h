#pragma once

//TODO déplacer dans le bon répertoire
namespace EnOceanMessage
{
   enum
   {
      kOffsetSyncByte = 0,
      kOffsetDataLength = 1,
      kOffsetOptionalLength = 3,
      kOffsetPacketType = 4,
      kOffsetCrc8Header = 5,
      kOffsetData = 6,
   };

   static const auto SYNC_BYTE_VALUE = 0x55;

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

   class CHeader
   {
   public:
      explicit CHeader(const std::vector<unsigned char>& receivedBuffer);

      unsigned short dataLength() const;
      unsigned char optionalLength() const;
      EPacketType packetType() const;
      int offsetData() const;
      int offsetOptional() const;

   private:
      const unsigned short m_dataLength;
      const unsigned char m_optionalLength;
      const EPacketType m_packetType;
      const int m_offsetData;
      const int m_offsetOptional;
   };


   inline unsigned short toWord(const std::vector<unsigned char>& content,
      const unsigned int offset)
   {
      return content[offset] + content[offset + 1];
   }


   //--------------------------------------------------------------
   /// \brief	EnOcean message
   ///
   /// This class manages a EnOcean message.
   //--------------------------------------------------------------
   class CMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                           Constructor
      /// \param[in] receivedContent         The received buffer
      //--------------------------------------------------------------
      explicit CMessage(const std::vector<unsigned char>& receivedBuffer);

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CMessage();

      const CHeader& header() const;
      const std::vector<unsigned char>& data() const;
      const std::vector<unsigned char>& optional() const;

   private:
      const CHeader m_header;
      const std::vector<unsigned char> m_data;
      const std::vector<unsigned char> m_optional;
   };
} // namespace EnOceanMessage
