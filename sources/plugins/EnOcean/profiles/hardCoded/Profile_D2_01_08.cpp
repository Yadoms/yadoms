#include "stdafx.h"
#include "Profile_D2_01_08.h"
#include "../bitsetHelpers.hpp"
#include "../../message/RadioErp1SendMessage.h"
#include "Profile_D2_01_Common.h"
#include <shared/Log.h>

CProfile_D2_01_08::CProfile_D2_01_08(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_channel(boost::make_shared<yApi::historization::CSwitch>("Channel", yApi::EKeywordAccessMode::kGetSet)),
     m_loadEnergy(boost::make_shared<yApi::historization::CEnergy>("Load energy")),
     m_loadPower(boost::make_shared<yApi::historization::CPower>("Load power")),
     m_overCurrent(boost::make_shared<yApi::historization::CSwitch>("OverCurrent", yApi::EKeywordAccessMode::kGetSet)),
     m_historizers({m_channel, m_loadEnergy, m_loadPower, m_overCurrent})
{
}

CProfile_D2_01_08::~CProfile_D2_01_08()
{
}

const std::string& CProfile_D2_01_08::profile() const
{
   static const std::string profile("D2-01-08");
   return profile;
}

const std::string& CProfile_D2_01_08::title() const
{
   static const std::string title("Electronic switch with energy measurement and local control");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_08::allHistorizers() const
{
   return m_historizers;
}

void CProfile_D2_01_08::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   CProfile_D2_01_Common::sendActuatorStatusQuery(messageHandler,
                                                  senderId,
                                                  m_deviceId,
                                                  CProfile_D2_01_Common::kOutputChannel1);
   CProfile_D2_01_Common::sendActuatorMeasurementQuery(messageHandler,
                                                       senderId,
                                                       m_deviceId,
                                                       CProfile_D2_01_Common::kOutputChannel1,
                                                       CProfile_D2_01_Common::kQueryPower);
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_08::states(unsigned char rorg,
                                                                                                   const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status,
                                                                                                   const std::string& senderId,
                                                                                                   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // This device supports several RORG messages
   // We just use the VLD telegram
   if (rorg != CRorgs::ERorgIds::kVLD_Telegram)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   switch (bitset_extract(data, 4, 4))
   {
   case CProfile_D2_01_Common::kActuatorStatusResponse:
      {
         return CProfile_D2_01_Common::extractActuatorStatusResponse(rorg,
                                                                     data,
                                                                     m_channel,
                                                                     CProfile_D2_01_Common::noChannel2,
                                                                     CProfile_D2_01_Common::noDimmable,
                                                                     CProfile_D2_01_Common::noPowerFailure,
                                                                     m_overCurrent);
      }
   case CProfile_D2_01_Common::kActuatorMeasurementResponse:
      {
         auto historizers = CProfile_D2_01_Common::extractActuatorMeasurementResponse(rorg,
                                                                                      data,
                                                                                      m_loadEnergy,
                                                                                      m_loadPower);

         if (std::find(historizers.begin(), historizers.end(), m_loadPower) != historizers.end())
         {
            // Power is configured to be received automaticaly.
            // As we can not receive both data (power + energy) automaticaly,
            // we ask for Energy just after receiving Power.
            CProfile_D2_01_Common::sendActuatorMeasurementQuery(messageHandler,
                                                                senderId,
                                                                m_deviceId,
                                                                CProfile_D2_01_Common::kOutputChannel1,
                                                                CProfile_D2_01_Common::kQueryEnergy);
         }

         return historizers;
      }
   default:
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }
}

void CProfile_D2_01_08::sendCommand(const std::string& keyword,
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

void CProfile_D2_01_08::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   auto localControl = deviceConfiguration.get<std::string>("localControl") == "enable";
   auto taughtInAllDevices = deviceConfiguration.get<std::string>("taughtIn") == "allDevices";
   auto userInterfaceDayMode = deviceConfiguration.get<std::string>("userInterfaceMode") == "dayMode";
   auto defaultState = deviceConfiguration.get<CProfile_D2_01_Common::EDefaultState>("defaultState");

   CProfile_D2_01_Common::sendActuatorSetLocalCommand(messageHandler,
                                                      senderId,
                                                      m_deviceId,
                                                      CProfile_D2_01_Common::kOutputChannel1,
                                                      localControl,
                                                      taughtInAllDevices,
                                                      userInterfaceDayMode,
                                                      false,
                                                      defaultState,
                                                      0.0,
                                                      0.0,
                                                      0.0);


   auto minEnergyMeasureRefreshTime = deviceConfiguration.get<double>("minEnergyMeasureRefreshTime");
   auto maxEnergyMeasureRefreshTime = deviceConfiguration.get<double>("maxEnergyMeasureRefreshTime");

   if (minEnergyMeasureRefreshTime > maxEnergyMeasureRefreshTime)
   {
      std::ostringstream oss;
      oss << "Min refresh time (" << minEnergyMeasureRefreshTime << ") is over max refresh time (" << maxEnergyMeasureRefreshTime << ") for device " << m_deviceId << " (" << profile() << ")";
      YADOMS_LOG(error) << oss.str();
      throw std::logic_error(oss.str());
   }

   // Configure for automatic power measure
   // At each power measure receive, we ask for energy measure
   CProfile_D2_01_Common::sendActuatorSetMeasurementCommand(messageHandler,
                                                            senderId,
                                                            m_deviceId,
                                                            CProfile_D2_01_Common::kOutputChannel1,
                                                            true,
                                                            minEnergyMeasureRefreshTime,
                                                            maxEnergyMeasureRefreshTime);
}
