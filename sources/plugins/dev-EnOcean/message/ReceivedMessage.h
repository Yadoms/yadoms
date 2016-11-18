#pragma once
#include "types.h"
#include "../profiles/generated-eep.h"

//TODO 3 classes dans ce fichier, faire le ménage
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
   /// \brief	EnOcean ESP3 received message
   ///
   /// This class manages an ESP3 EnOcean message, when receiving.
   //--------------------------------------------------------------
   class CReceivedEsp3Packet
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                           Constructor
      /// \param[in] receivedBuffer          The received buffer
      //--------------------------------------------------------------
      explicit CReceivedEsp3Packet(const std::vector<unsigned char>& receivedBuffer);

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CReceivedEsp3Packet();

      const CHeader& header() const;
      const std::vector<unsigned char>& data() const;
      const std::vector<unsigned char>& optional() const;

   private:
      const CHeader m_header;
      const std::vector<unsigned char> m_data;
      const std::vector<unsigned char> m_optional;
   };


   //--------------------------------------------------------------
   /// \brief	EnOcean RADIO_ERP1 message
   ///
   /// This class manages a RADIO_ERP1 EnOcean message.
   //--------------------------------------------------------------
   class CRadioErp1Message
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                           Constructor
      /// \param[in] esp3Packet              The esp3 message
      //--------------------------------------------------------------
      explicit CRadioErp1Message(const CReceivedEsp3Packet& esp3Packet);

      //--------------------------------------------------------------
      /// \brief	                           Destructor
      //--------------------------------------------------------------
      virtual ~CRadioErp1Message();

      CRorgs::ERorgIds rorg() const;
      unsigned int senderId() const;
      std::string senderIdAsString() const;
      const std::vector<unsigned char>& data() const;
      unsigned char status() const;
      std::string dump() const;
   protected:
      static std::string senderIdToString(unsigned int senderId);
   private:
      const CRorgs::ERorgIds m_rorg;
      const unsigned int m_senderId;
      const std::string m_senderIdAsString;
      const unsigned char m_status;
      const std::vector<unsigned char> m_data;
   };


} // namespace message
