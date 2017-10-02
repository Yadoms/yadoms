#include "stdafx.h"
#include "Profile_D2_01_04.h"
#include "../bitsetHelpers.hpp"
#include "../../message/RadioErp1SendMessage.h"
#include "Profile_D2_01_Common.h"
#include <shared/Log.h>

CProfile_D2_01_04::CProfile_D2_01_04(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_dimmerMode(boost::make_shared<specificHistorizers::CDimmerModeHistorizer>("DimmerMode")),
     m_dimmer(boost::make_shared<yApi::historization::CDimmable>("Dimmer", yApi::EKeywordAccessMode::kGetSet)),
     m_historizers({m_dimmer , m_dimmerMode})
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
   int dimValue = bitset_extract(data, 17, 7);
   switch (ioChannel)
   {
   case 0:
      m_dimmer->set(dimValue);
      historizers.push_back(m_dimmer);
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
   if (keyword == m_dimmer->getKeyword())
   {
      m_dimmer->setCommand(commandBody);
   }
   else if (keyword == m_dimmerMode->getKeyword())
   {
      m_dimmerMode->setCommand(commandBody);
      // Nothing to do more, this keyword is at internal-usage only.
      // It will be used at next dimmer value change.
      return;
   }
   else
      return;

   CProfile_D2_01_Common::sendActuatorSetOutputCommandDimming(messageHandler,
                                                              senderId,
                                                              m_deviceId,
                                                              CProfile_D2_01_Common::kOutputChannel1,
                                                              m_dimmerMode->get(),
                                                              m_dimmer->get());
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
                                                      CProfile_D2_01_Common::kOutputChannel1,
                                                      localControl,
                                                      taughtInAllDevices,
                                                      false,
                                                      false,
                                                      CProfile_D2_01_Common::EDefaultState::knotUsed,
                                                      dimTimer1,
                                                      dimTimer2,
                                                      dimTimer3);
}
