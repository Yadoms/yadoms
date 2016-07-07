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
   class CSendMessage
   {
   protected:
      explicit CSendMessage(EPacketType packetType); // Constructed only by children

   public:
      virtual ~CSendMessage();

      EPacketType packetType() const;
      shared::communication::CByteBuffer buildBuffer() const;

      virtual void appendData(const std::vector<unsigned char>& data);
      virtual void appendOptional(const std::vector<unsigned char>& data);

   protected:
      virtual void updateBuffer(std::vector<unsigned char>& buffer) const = 0;

      const EPacketType m_packetType;
      std::vector<unsigned char> m_data;
      std::vector<unsigned char> m_optional;
   };


   //--------------------------------------------------------------
   /// \brief	EnOcean send command message
   //--------------------------------------------------------------
   class CCommandSendMessage : public CSendMessage// TODO Déplacer
   {
   public:
      CCommandSendMessage();
      virtual ~CCommandSendMessage();

      void updateBuffer(std::vector<unsigned char>& buffer) const override;
      
   };
} // namespace message
