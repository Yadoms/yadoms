#include "stdafx.h"
#include "Profile_A5_11_05.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"
#include "message/RadioErp1SendMessage.h"
#include "message/ResponseReceivedMessage.h"

CProfile_A5_11_05::CProfile_A5_11_05(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_channel1(boost::make_shared<yApi::historization::CSwitch>("Channel 1", yApi::EKeywordAccessMode::kGet)),
     m_channel2(boost::make_shared<yApi::historization::CSwitch>("Channel 2", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_channel1, m_channel2})
{
}

const std::string& CProfile_A5_11_05::profile() const
{
   static const std::string Profile("A5-11-05");
   return Profile;
}

const std::string& CProfile_A5_11_05::title() const
{
   static const std::string Title(
      "Controller Status - Dual-Channel Switch Actuator (BI-DIR)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_11_05::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_11_05::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   boost::dynamic_bitset<> userData(4 * 8);

   bitset_insert(userData, 28, 1, 1);
   bitset_insert(userData, 31, 1, 0);

   message::CRadioErp1SendMessage command(CRorgs::k4BS_Telegram,
                                          senderId,
                                          m_deviceId,
                                          0);

   command.userData(bitset_to_bytes(userData));

   static const auto CommandName("Gateway request telegram");
   boost::shared_ptr<const message::CEsp3ReceivedPacket> answer;
   if (!messageHandler->send(command,
                             [](boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet)
                             {
                                return esp3Packet->header().packetType() == message::RESPONSE;
                             },
                             [&](boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet)
                             {
                                answer = esp3Packet;
                             }))
      throw std::runtime_error(
         (boost::format("Fail to send message to %1% : no answer to \"%2%\"") % m_deviceId % CommandName).str());

   const auto response = boost::make_shared<message::CResponseReceivedMessage>(answer);

   if (response->returnCode() != message::CResponseReceivedMessage::RET_OK)
      YADOMS_LOG(error) << "Fail to send message to " << m_deviceId << " : \"" << CommandName << "\" returns " <<
         response->returnCode();
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_11_05::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   switch (bitset_extract(data, 29, 2))
   {
   case 0:
      m_channel1->set(false);
      m_channel2->set(false);
      break;
   case 1:
      m_channel1->set(true);
      m_channel2->set(false);
      break;
   case 2:
      m_channel1->set(false);
      m_channel2->set(true);
      break;
   case 3:
      m_channel1->set(true);
      m_channel2->set(true);
      break;
   default:
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   return m_historizers;
}

void CProfile_A5_11_05::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_11_05::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
