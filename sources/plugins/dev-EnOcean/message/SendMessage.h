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
      void updateBuffer(std::vector<unsigned char>& buffer) const;

      const EPacketType m_packetType;
      std::vector<unsigned char> m_data;
      std::vector<unsigned char> m_optional;
   };


   //--------------------------------------------------------------
   /// \brief	EnOcean send command message
   //--------------------------------------------------------------
   class CCommonCommandSendMessage : public CEsp3SendPacket// TODO Déplacer
   {
   public:
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

      CCommonCommandSendMessage();
      virtual ~CCommonCommandSendMessage();   
   };


   //--------------------------------------------------------------
   /// \brief	EnOcean send command message
   //--------------------------------------------------------------
   class CRadioErp1SendMessage : public CEsp3SendPacket// TODO Déplacer
   {
   public:
      CRadioErp1SendMessage();
      virtual ~CRadioErp1SendMessage();
   };
} // namespace message
