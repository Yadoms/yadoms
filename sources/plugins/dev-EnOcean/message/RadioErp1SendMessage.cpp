#include "stdafx.h"
#include "RadioErp1SendMessage.h"

namespace message
{
   CRadioErp1SendMessage::CRadioErp1SendMessage(CRorgs::ERorgIds rorg,
                                                const std::string& senderId,
                                                unsigned char status)
      : CEsp3SendPacket(RADIO_ERP1),
        m_rorg(rorg),
        m_senderId(senderId),
        m_status(status)
   {
   }

   CRadioErp1SendMessage::~CRadioErp1SendMessage()
   {
   }

   void CRadioErp1SendMessage::userData(const std::vector<unsigned char>& userData)
   {
      m_userData = userData;
   }

   boost::shared_ptr<const std::vector<unsigned char>> CRadioErp1SendMessage::buffer()
   {
      auto data = std::vector<unsigned char>(1 + m_userData.size() + 4 + 1);

      size_t bufferIndex = 0;
      data[bufferIndex++] = m_rorg;
      for (const auto userDataByte: m_userData)
         data[bufferIndex++] = userDataByte;
      data[bufferIndex++] = static_cast<unsigned char>(std::stoul(m_senderId.substr(0, 2), nullptr, 16));
      data[bufferIndex++] = static_cast<unsigned char>(std::stoul(m_senderId.substr(2, 2), nullptr, 16));
      data[bufferIndex++] = static_cast<unsigned char>(std::stoul(m_senderId.substr(4, 2), nullptr, 16));
      data[bufferIndex++] = static_cast<unsigned char>(std::stoul(m_senderId.substr(6, 2), nullptr, 16));
      data[bufferIndex] = m_status;

      CEsp3SendPacket::data(data);

      return CEsp3SendPacket::buffer();
   }
} // namespace message
