#include "stdafx.h"
#include "Profile_D2_01_04.h"
#include "../bitsetHelpers.hpp"
#include "../../message/RadioErp1SendMessage.h"
#include "../../message/ResponseReceivedMessage.h"
#include "Profile_D2_01_Common.h"
#include <shared/Log.h>

CProfile_D2_01_04::CProfile_D2_01_04(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_dimAtSpeed1(boost::make_shared<yApi::historization::CDimmable>("Dim at speed 1", yApi::EKeywordAccessMode::kGetSet)),
     m_dimAtSpeed2(boost::make_shared<yApi::historization::CDimmable>("Dim at speed 2", yApi::EKeywordAccessMode::kGetSet)),
     m_dimAtSpeed3(boost::make_shared<yApi::historization::CDimmable>("Dim at speed 3", yApi::EKeywordAccessMode::kGetSet)),
     m_historizers({m_dimAtSpeed1 , m_dimAtSpeed2 , m_dimAtSpeed3})
{
}

CProfile_D2_01_04::~CProfile_D2_01_04()
{
}

const std::string& CProfile_D2_01_04::profile() const
{
   static const std::string profile("D2-01-04");
   return profile;
}

const std::string& CProfile_D2_01_04::title() const
{
   static const std::string title("Electronic dimmer with energy measurement and local control");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_04::allHistorizers() const
{
   return m_historizers;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_04::states(unsigned char rorg,
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

void CProfile_D2_01_04::sendCommand(const std::string& keyword,
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

   boost::dynamic_bitset<> userData(3 * 8);
   bitset_insert(userData, 4, 4, CProfile_D2_01_Common::kActuatorSetOutput);
   bitset_insert(userData, 8, 3, dimValue);
   bitset_insert(userData, 11, 5, 0);
   bitset_insert(userData, 17, 7, std::stoul(commandBody));

   CProfile_D2_01_Common::sendMessage(messageHandler,
                                      senderId,
                                      m_deviceId,
                                      userData,
                                      "Actuator Set Output");
}

void CProfile_D2_01_04::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   auto localControl = deviceConfiguration.get<std::string>("localControl") == "enable";
   auto taughtInAllDevices = deviceConfiguration.get<std::string>("taughtIn") == "allDevices";
   auto dimTimer1 = deviceConfiguration.get<double>("dimTimer1");
   auto dimTimer2 = deviceConfiguration.get<double>("dimTimer2");
   auto dimTimer3 = deviceConfiguration.get<double>("dimTimer3");

   // CMD 0x2 - Actuator Set Local
   CProfile_D2_01_Common::sendActuatorSetLocalCommand(messageHandler,
                                                      senderId,
                                                      m_deviceId,
                                                      localControl,
                                                      taughtInAllDevices,
                                                      false,
                                                      false,
                                                      CProfile_D2_01_Common::EDefaultState::knotUsed,
                                                      dimTimer1,
                                                      dimTimer2,
                                                      dimTimer3);
}

