#pragma once
#include "types.h"
#include <shared/communication/Buffer.hpp>

namespace message
{
   //--------------------------------------------------------------
   /// \brief	EnOcean send message
   ///
   /// This class manages a EnOcean message.
   //--------------------------------------------------------------
   class CEsp3SendPacket
   {
   protected:
      explicit CEsp3SendPacket(EPacketType packetType); // Constructed only by children

   public:
      virtual ~CEsp3SendPacket();

      EPacketType packetType() const;
      shared::communication::CByteBuffer buildBuffer() const;

      virtual void appendData(const std::vector<unsigned char>& data);
      virtual void appendOptional(const std::vector<unsigned char>& data);

   protected:
      virtual void updateBuffer(std::vector<unsigned char>& buffer) const;

      const EPacketType m_packetType;
      std::vector<unsigned char> m_data;
      std::vector<unsigned char> m_optional;
   };
} // namespace message
