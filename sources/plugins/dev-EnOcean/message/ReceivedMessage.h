#pragma once
#include "types.h"

namespace message
{
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
   /// \brief	EnOcean received message
   ///
   /// This class manages a EnOcean message.
   //--------------------------------------------------------------
   class CReceivedMessage
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                           Constructor
      /// \param[in] receivedContent         The received buffer
      //--------------------------------------------------------------
      explicit CReceivedMessage(const std::vector<unsigned char>& receivedBuffer);

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CReceivedMessage();

      const CHeader& header() const;
      const std::vector<unsigned char>& data() const;
      const std::vector<unsigned char>& optional() const;

   private:
      const CHeader m_header;
      const std::vector<unsigned char> m_data;
      const std::vector<unsigned char> m_optional;
   };
} // namespace message
