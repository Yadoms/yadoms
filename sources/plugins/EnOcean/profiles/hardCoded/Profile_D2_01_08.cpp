#include "stdafx.h"
#include "Profile_D2_01_08.h"
#include "../bitsetHelpers.hpp"
#include "../../message/RadioErp1SendMessage.h"
#include <shared/Log.h>

CProfile_D2_01_08::CProfile_D2_01_08(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_channel(boost::make_shared<yApi::historization::CSwitch>("Channel", yApi::EKeywordAccessMode::kGetSet)),
     m_loadEnergy(boost::make_shared<yApi::historization::CEnergy>("Load energy")),
     m_resetLoadEnergy(boost::make_shared<yApi::historization::CEvent>("ResetLoadEnergy")),
     m_loadPower(boost::make_shared<yApi::historization::CPower>("Load power")),
     m_overCurrent(boost::make_shared<yApi::historization::CSwitch>("OverCurrent", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_channel, m_loadEnergy, m_resetLoadEnergy, m_loadPower, m_overCurrent}),
     m_outputChannel(CProfile_D2_01_Common::kOutputChannel1)
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
   // Be careful : manufacturers implementations of EnOcean EEP specifications differ :
   // some support only kAllOutputChannels (0x1E) to drive output (like Nodon alias ID_RF), some other support only
   // kOutputChannel1 to drive output (0x00) to drive output (like GIGA-concept).
   // So send the 2 messages and try to memorize the one device responds to.
   // Note also that som devices (like GIGA-concept DO21-12B smart-plug) doesn't return its channel state
   // (never send kActuatorStatusResponse message), so default value must be kOutputChannel1.

   // Need to wait a bit between outgoing messages, to be sure to receive answer
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));
   CProfile_D2_01_Common::sendActuatorStatusQuery(messageHandler,
                                                  senderId,
                                                  m_deviceId,
                                                  CProfile_D2_01_Common::kAllOutputChannels);

   boost::this_thread::sleep(boost::posix_time::milliseconds(500));
   CProfile_D2_01_Common::sendActuatorStatusQuery(messageHandler,
                                                  senderId,
                                                  m_deviceId,
                                                  CProfile_D2_01_Common::kOutputChannel1);


   // Need to wait a bit between outgoing messages, to be sure to receive answer
   boost::this_thread::sleep(boost::posix_time::milliseconds(500));
   CProfile_D2_01_Common::sendActuatorMeasurementQuery(messageHandler,
                                                       senderId,
                                                       m_deviceId,
                                                       CProfile_D2_01_Common::kAllOutputChannels,
                                                       CProfile_D2_01_Common::kQueryPower);
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_08::states(
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
   case CProfile_D2_01_Common::kActuatorStatusResponse:
      {
         // Learn output channel to use
         auto receiveOutputChannel = bitset_extract(data, 11, 5);
         if (receiveOutputChannel != CProfile_D2_01_Common::kInputChannel)
            m_outputChannel = static_cast<CProfile_D2_01_Common::EOutputChannel>(receiveOutputChannel);

         return CProfile_D2_01_Common::extractActuatorStatusResponse(rorg,
                                                                     data,
                                                                     m_channel,
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
            // Power is configured to be received automatically.
            // As we can not receive both data (power + energy) automatically,
            // we ask for Energy just after receiving Power.
            CProfile_D2_01_Common::sendActuatorMeasurementQuery(messageHandler,
                                                                senderId,
                                                                m_deviceId,
                                                                CProfile_D2_01_Common::kAllOutputChannels,
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
   if (keyword == m_channel->getKeyword())
   {
      m_channel->setCommand(commandBody);

      CProfile_D2_01_Common::sendActuatorSetOutputCommandSwitching(messageHandler,
                                                                   senderId,
                                                                   m_deviceId,
                                                                   m_outputChannel,
                                                                   m_channel->get());
   }
   else if (keyword == m_resetLoadEnergy->getKeyword())
   {
      // Resetting the load energy counter is done by sending an Actuator Set Measurement command
      CProfile_D2_01_Common::sendActuatorSetMeasurementCommand(messageHandler,
                                                               senderId,
                                                               m_deviceId,
                                                               m_outputChannel,
                                                               false,
                                                               true,
                                                               0,
                                                               0);

      // Ask for a new energy measure now to reflect de resetted value      

      // Need to wait a bit between outgoing messages, to be sure to receive answer
      boost::this_thread::sleep(boost::posix_time::milliseconds(500));
      CProfile_D2_01_Common::sendActuatorMeasurementQuery(messageHandler,
                                                          senderId,
                                                          m_deviceId,
                                                          m_outputChannel,
                                                          CProfile_D2_01_Common::kQueryPower);
   }
}

void CProfile_D2_01_08::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
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
                                                      m_outputChannel,
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
                                                            m_outputChannel,
                                                            true,
                                                            false,
                                                            minEnergyMeasureRefreshTime,
                                                            maxEnergyMeasureRefreshTime);
}
