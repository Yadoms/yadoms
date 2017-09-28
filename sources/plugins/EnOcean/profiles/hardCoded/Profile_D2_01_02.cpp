#include "stdafx.h"
#include "Profile_D2_01_02.h"
#include "../bitsetHelpers.hpp"
#include "../../message/RadioErp1SendMessage.h"
#include "../../message/ResponseReceivedMessage.h"
#include "Profile_D2_01_Common.h"
#include <shared/Log.h>

CProfile_D2_01_02::CProfile_D2_01_02(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_dimAtSpeed1(boost::make_shared<yApi::historization::CDimmable>("Dim at speed 1", yApi::EKeywordAccessMode::kGetSet)),
     m_dimAtSpeed2(boost::make_shared<yApi::historization::CDimmable>("Dim at speed 2", yApi::EKeywordAccessMode::kGetSet)),
     m_dimAtSpeed3(boost::make_shared<yApi::historization::CDimmable>("Dim at speed 3", yApi::EKeywordAccessMode::kGetSet)),
     m_historizers({m_dimAtSpeed1 , m_dimAtSpeed2 , m_dimAtSpeed3})
{
}

CProfile_D2_01_02::~CProfile_D2_01_02()
{
}

const std::string& CProfile_D2_01_02::profile() const
{
   static const std::string profile("D2-01-02");
   return profile;
}

const std::string& CProfile_D2_01_02::title() const
{
   static const std::string title("Electronic dimmer with local Control");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_02::allHistorizers() const
{
   return m_historizers;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_02::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status,
                                                                                                   const std::string& senderId,
                                                                                                   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // This device supports several RORG messages
   // We just use the VLD telegram
   if (rorg != CRorgs::ERorgIds::kVLD_Telegram)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   if (bitset_extract(data, 4, 4) != CProfile_D2_01_Common::kActuatorStatusResponse)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   // Return only the concerned historizer
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   auto ioChannel = bitset_extract(data, 11, 5);
   auto state = bitset_extract(data, 17, 1) ? true : false;
   switch (ioChannel)
   {
   case 0:
      m_dimAtSpeed1->set(state);
      m_dimAtSpeed2->set(state);
      m_dimAtSpeed3->set(state);
      historizers.push_back(m_dimAtSpeed1);
      historizers.push_back(m_dimAtSpeed2);
      historizers.push_back(m_dimAtSpeed3);
      break;
   default:
      YADOMS_LOG(information) << "Profile " << profile() << " : received unsupported ioChannel value " << ioChannel;
      break;
   }
   return historizers;
}

void CProfile_D2_01_02::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   CProfile_D2_01_Common::E_D2_01_DimValue dimValue;
   if (keyword == m_dimAtSpeed1->getKeyword())
   {
      dimValue = CProfile_D2_01_Common::kDimToValueWithTimer1;
      m_dimAtSpeed2->set(m_dimAtSpeed1->switchLevel());
      m_dimAtSpeed3->set(m_dimAtSpeed1->switchLevel());
   }
   else if (keyword == m_dimAtSpeed2->getKeyword())
   {
      dimValue = CProfile_D2_01_Common::kDimToValueWithTimer2;
      m_dimAtSpeed1->set(m_dimAtSpeed2->switchLevel());
      m_dimAtSpeed3->set(m_dimAtSpeed2->switchLevel());
   }
   else if (keyword == m_dimAtSpeed3->getKeyword())
   {
      dimValue = CProfile_D2_01_Common::kDimToValueWithTimer3;
      m_dimAtSpeed1->set(m_dimAtSpeed3->switchLevel());
      m_dimAtSpeed2->set(m_dimAtSpeed3->switchLevel());
   }
   else
   {
      std::ostringstream oss;
      oss << "Device " << m_deviceId << " (" << profile() << ") : send command on unsupported keyword " << keyword;
      YADOMS_LOG(information) << oss.str();
      throw std::logic_error(oss.str());
   }

   message::CRadioErp1SendMessage command(CRorgs::kVLD_Telegram,
                                          senderId,
                                          m_deviceId,
                                          0);

   boost::dynamic_bitset<> userData(3 * 8);
   bitset_insert(userData, 4, 4, CProfile_D2_01_Common::kActuatorSetOutput);
   bitset_insert(userData, 8, 3, dimValue);
   bitset_insert(userData, 11, 5, 0);
   bitset_insert(userData, 17, 7, std::stoul(commandBody));

   command.userData(bitset_to_bytes(userData));

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
   YADOMS_LOG(error) << "Fail to send state to " << m_deviceId << " : no answer to Actuator Set Output command";

   auto response = boost::make_shared<message::CResponseReceivedMessage>(answer);

   if (response->returnCode() != message::CResponseReceivedMessage::RET_OK)
   YADOMS_LOG(error) << "Fail to send state to " << m_deviceId << " : Actuator Set Output command returns " << response->returnCode();
}

void CProfile_D2_01_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
