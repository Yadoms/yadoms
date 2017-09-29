#include "stdafx.h"
#include "Profile_D2_01_11.h"
#include "../bitsetHelpers.hpp"
#include "../../message/RadioErp1SendMessage.h"
#include "../../message/ResponseReceivedMessage.h"
#include "Profile_D2_01_Common.h"
#include <shared/Log.h>

CProfile_D2_01_11::CProfile_D2_01_11(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_channel1(boost::make_shared<yApi::historization::CSwitch>("Channel 1", yApi::EKeywordAccessMode::kGetSet)),
     m_channel2(boost::make_shared<yApi::historization::CSwitch>("Channel 2", yApi::EKeywordAccessMode::kGetSet)),
     m_historizers({m_channel1 , m_channel2})
{
}

CProfile_D2_01_11::~CProfile_D2_01_11()
{
}

const std::string& CProfile_D2_01_11::profile() const
{
   static const std::string profile("D2-01-11");
   return profile;
}

const std::string& CProfile_D2_01_11::title() const
{
   static const std::string title("Electronic switch (2 channels) with local control");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_11::allHistorizers() const
{
   return m_historizers;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_11::states(unsigned char rorg,
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
      m_channel1->set(state);
      historizers.push_back(m_channel1);
      break;
   case 1:
      m_channel2->set(state);
      historizers.push_back(m_channel2);
      break;
   default:
      YADOMS_LOG(information) << "Profile " << profile() << " : received unsupported ioChannel value " << ioChannel;
      break;
   }
   return historizers;
}

void CProfile_D2_01_11::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   CProfile_D2_01_Common::sendActuatorSetOutputCommandSwitching(messageHandler,
                                                                senderId,
                                                                m_deviceId,
                                                                keyword == m_channel1->getKeyword()
                                                                   ? CProfile_D2_01_Common::kOutputChannel1
                                                                   : CProfile_D2_01_Common::kOutputChannel2,
                                                                commandBody == "1");
}

void CProfile_D2_01_11::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
