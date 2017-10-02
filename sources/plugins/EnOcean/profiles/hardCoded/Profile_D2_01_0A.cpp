#include "stdafx.h"
#include "Profile_D2_01_0A.h"
#include "../bitsetHelpers.hpp"
#include "../../message/RadioErp1SendMessage.h"
#include "Profile_D2_01_Common.h"
#include <shared/Log.h>

CProfile_D2_01_0A::CProfile_D2_01_0A(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_channel(boost::make_shared<yApi::historization::CSwitch>("Channel", yApi::EKeywordAccessMode::kGetSet)),
     m_powerFailure(boost::make_shared<yApi::historization::CSwitch>("Power failure", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_channel, m_powerFailure})
{
}

CProfile_D2_01_0A::~CProfile_D2_01_0A()
{
}

const std::string& CProfile_D2_01_0A::profile() const
{
   static const std::string profile("D2-01-0A");
   return profile;
}

const std::string& CProfile_D2_01_0A::title() const
{
   static const std::string title("Electronic switch with local control");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0A::allHistorizers() const
{
   return m_historizers;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0A::states(unsigned char rorg,
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
      m_channel->set(state);
      historizers.push_back(m_channel);
      break;
   default:
      YADOMS_LOG(information) << "Profile " << profile() << " : received unsupported ioChannel value " << ioChannel;
      break;
   }

   auto powerFailureSupported = bitset_extract(data, 0, 1) ? true : false;
   auto powerFailureState = bitset_extract(data, 1, 1) ? true : false;
   if (powerFailureSupported)
   {
      m_powerFailure->set(powerFailureState);
      historizers.push_back(m_powerFailure);
   }

   return historizers;
}

void CProfile_D2_01_0A::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   if (keyword != m_channel->getKeyword())
      return;

   m_channel->setCommand(commandBody);

   CProfile_D2_01_Common::sendActuatorSetOutputCommandSwitching(messageHandler,
                                                                senderId,
                                                                m_deviceId,
                                                                CProfile_D2_01_Common::kOutputChannel1,
                                                                m_channel->get());
}

void CProfile_D2_01_0A::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   auto localControl = deviceConfiguration.get<std::string>("localControl") == "enable";
   auto taughtInAllDevices = deviceConfiguration.get<std::string>("taughtIn") == "allDevices";
   auto userInterfaceDayMode = deviceConfiguration.get<std::string>("userInterfaceMode") == "dayMode";
   auto defaultState = deviceConfiguration.get<CProfile_D2_01_Common::EDefaultState>("defaultState");
   auto powerFailureDetection = deviceConfiguration.get<std::string>("powerFailureDetection") == "enable";

   CProfile_D2_01_Common::sendActuatorSetLocalCommand(messageHandler,
                                                      senderId,
                                                      m_deviceId,
                                                      CProfile_D2_01_Common::kOutputChannel1,
                                                      localControl,
                                                      taughtInAllDevices,
                                                      userInterfaceDayMode,
                                                      powerFailureDetection,
                                                      defaultState,
                                                      0.0,
                                                      0.0,
                                                      0.0);
}
