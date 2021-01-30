#include "stdafx.h"
#include "Profile_D2_01_0C.h"
#include "../bitsetHelpers.hpp"
#include "../../message/RadioErp1SendMessage.h"
#include "Profile_D2_01_Common.h"
#include <shared/Log.h>

CProfile_D2_01_0C::CProfile_D2_01_0C(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_channel(boost::make_shared<yApi::historization::CSwitch>("Channel", yApi::EKeywordAccessMode::kGetSet)),
     m_loadEnergy(boost::make_shared<yApi::historization::CEnergy>("Load energy")),
     m_resetLoadEnergy(boost::make_shared<yApi::historization::CEvent>("ResetLoadEnergy")),
     m_loadPower(boost::make_shared<yApi::historization::CPower>("Load power")),
     m_pilotWire(boost::make_shared<specificHistorizers::CPilotWireHistorizer>("Pilot wire")),
     m_overCurrent(boost::make_shared<yApi::historization::CSwitch>("OverCurrent", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_channel, m_loadEnergy, m_resetLoadEnergy, m_loadPower, m_pilotWire, m_overCurrent})
{
}

const std::string& CProfile_D2_01_0C::profile() const
{
   static const std::string profile("D2-01-0C");
   return profile;
}

const std::string& CProfile_D2_01_0C::title() const
{
   static const std::string title("Electronic switch with energy measurement and local control");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0C::allHistorizers() const
{
   return m_historizers;
}

void CProfile_D2_01_0C::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Need to wait a bit between outgoing messages, to be sure to receive answer
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));
   CProfile_D2_01_Common::sendActuatorStatusQuery(messageHandler,
                                                  senderId,
                                                  m_deviceId,
                                                  CProfile_D2_01_Common::EOutputChannel::kAllOutputChannels);

   // Need to wait a bit between outgoing messages, to be sure to receive answer
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));
   CProfile_D2_01_Common::sendActuatorMeasurementQuery(messageHandler,
                                                       senderId,
                                                       m_deviceId,
                                                       CProfile_D2_01_Common::EOutputChannel::kAllOutputChannels,
                                                       CProfile_D2_01_Common::EPowerQueryType::kQueryPower);

   // Need to wait a bit between outgoing messages, to be sure to receive answer
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));
   CProfile_D2_01_Common::sendActuatorPilotWireModeQuery(messageHandler,
                                                         senderId,
                                                         m_deviceId);
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0C::states(
   unsigned char rorg,
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
   case CProfile_D2_01_Common::E_D2_01_Command::kActuatorStatusResponse:
      {
         return CProfile_D2_01_Common::extractActuatorStatusResponse(rorg,
                                                                     data,
                                                                     m_channel,
                                                                     CProfile_D2_01_Common::noDimmable,
                                                                     CProfile_D2_01_Common::noPowerFailure,
                                                                     m_overCurrent);
      }
   case CProfile_D2_01_Common::E_D2_01_Command::kActuatorMeasurementResponse:
      {
         auto historizers = CProfile_D2_01_Common::extractActuatorMeasurementResponse(rorg,
                                                                                      data,
                                                                                      m_loadEnergy,
                                                                                      m_loadPower);

         if (std::find(historizers.begin(), historizers.end(), m_loadPower) != historizers.end())
         {
            // Power is configured to be received automatically.
            // As we can not receive both data (power + energy) automatically,
            // we ask for Energy just after receiving Power.
            CProfile_D2_01_Common::sendActuatorMeasurementQuery(messageHandler,
                                                                senderId,
                                                                m_deviceId,
                                                                CProfile_D2_01_Common::EOutputChannel::kAllOutputChannels,
                                                                CProfile_D2_01_Common::EPowerQueryType::kQueryEnergy);
         }

         return historizers;
      }
   case CProfile_D2_01_Common::E_D2_01_Command::kActuatorPilotWireModeResponse:
      {
         return CProfile_D2_01_Common::extractActuatorPilotWireModeResponse(rorg,
                                                                            data,
                                                                            m_pilotWire);
      }
   default:
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }
}

void CProfile_D2_01_0C::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   if (keyword == m_channel->getKeyword())
   {
      m_channel->setCommand(commandBody);
      CProfile_D2_01_Common::sendActuatorSetOutputCommandSwitching(messageHandler,
                                                                   senderId,
                                                                   m_deviceId,
                                                                   CProfile_D2_01_Common::EOutputChannel::kAllOutputChannels,
                                                                   m_channel->get());
   }
   else if (keyword == m_pilotWire->getKeyword())
   {
      m_pilotWire->setCommand(commandBody);
      CProfile_D2_01_Common::sendActuatorSetPilotWireModeCommand(messageHandler,
                                                                 senderId,
                                                                 m_deviceId,
                                                                 m_pilotWire->get());
   }
   else if (keyword == m_resetLoadEnergy->getKeyword())
   {
      // Resetting the load energy counter is done by sending an Actuator Set Measurement command
      CProfile_D2_01_Common::sendActuatorSetMeasurementCommand(messageHandler,
                                                               senderId,
                                                               m_deviceId,
                                                               CProfile_D2_01_Common::EOutputChannel::kAllOutputChannels,
                                                               true,
                                                               true,
                                                               0,
                                                               0);

      // Ask for a new energy measure now to reflect de resetted value      

      // Need to wait a bit between outgoing messages, to be sure to receive answer
      boost::this_thread::sleep(boost::posix_time::milliseconds(500));
      CProfile_D2_01_Common::sendActuatorMeasurementQuery(messageHandler,
                                                          senderId,
                                                          m_deviceId,
                                                          CProfile_D2_01_Common::EOutputChannel::kAllOutputChannels,
                                                          CProfile_D2_01_Common::EPowerQueryType::kQueryPower);
   }
}

void CProfile_D2_01_0C::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   const auto localControl = deviceConfiguration.get<std::string>("localControl") == "enable";
   const auto taughtInAllDevices = deviceConfiguration.get<std::string>("taughtIn") == "allDevices";
   const auto userInterfaceDayMode = deviceConfiguration.get<std::string>("userInterfaceMode") == "dayMode";
   const auto defaultState = deviceConfiguration.get<CProfile_D2_01_Common::EDefaultState>("defaultState");

   CProfile_D2_01_Common::sendActuatorSetLocalCommand(messageHandler,
                                                      senderId,
                                                      m_deviceId,
                                                      CProfile_D2_01_Common::EOutputChannel::kAllOutputChannels,
                                                      localControl,
                                                      taughtInAllDevices,
                                                      userInterfaceDayMode,
                                                      false,
                                                      defaultState,
                                                      0.0,
                                                      0.0,
                                                      0.0);


   const auto minEnergyMeasureRefreshTime = deviceConfiguration.get<double>("minEnergyMeasureRefreshTime");
   const auto maxEnergyMeasureRefreshTime = deviceConfiguration.get<double>("maxEnergyMeasureRefreshTime");

   if (minEnergyMeasureRefreshTime > maxEnergyMeasureRefreshTime)
   {
      std::ostringstream oss;
      oss << "Min refresh time (" << minEnergyMeasureRefreshTime << ") is over max refresh time (" << maxEnergyMeasureRefreshTime << ") for device "
         << m_deviceId << " (" << profile() << ")";
      YADOMS_LOG(error) << oss.str();
      throw std::logic_error(oss.str());
   }

   // Configure for automatic power measure
   // At each power measure receive, we ask for energy measure
   CProfile_D2_01_Common::sendActuatorSetMeasurementCommand(messageHandler,
                                                            senderId,
                                                            m_deviceId,
                                                            CProfile_D2_01_Common::EOutputChannel::kAllOutputChannels,
                                                            true,
                                                            false,
                                                            minEnergyMeasureRefreshTime,
                                                            maxEnergyMeasureRefreshTime);
}
