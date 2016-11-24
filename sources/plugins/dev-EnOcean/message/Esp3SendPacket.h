#pragma once
#include "types.h"

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

      void data(const std::vector<unsigned char>& data);
      void optionalData(const std::vector<unsigned char>& optionalData);

      virtual boost::shared_ptr<const std::vector<unsigned char>> buffer();

   protected:
      const EPacketType m_packetType;
      std::vector<unsigned char> m_data;
      std::vector<unsigned char> m_optionalData;
   };
} // namespace message
