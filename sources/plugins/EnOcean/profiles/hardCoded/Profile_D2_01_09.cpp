#include "stdafx.h"
#include "Profile_D2_01_09.h"
#include "../bitsetHelpers.hpp"
#include "../../message/radioErp1/SendMessage.h"
#include "Profile_D2_01_Common.h"
#include <shared/Log.h>

#include <utility>

CProfile_D2_01_09::CProfile_D2_01_09(std::string deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(std::move(deviceId)),
     m_loadEnergy(boost::make_shared<yApi::historization::CEnergy>("Load energy")),
     m_resetLoadEnergy(boost::make_shared<yApi::historization::CEvent>("ResetLoadEnergy")),
     m_loadPower(boost::make_shared<yApi::historization::CPower>("Load power")),
     m_dimmerMode(boost::make_shared<specificHistorizers::CDimmerModeHistorizer>("DimmerMode")),
     m_dimmer(boost::make_shared<yApi::historization::CDimmable>("Dimmer", yApi::EKeywordAccessMode::kGetSet)),
     m_overCurrent(boost::make_shared<yApi::historization::CSwitch>("OverCurrent", yApi::EKeywordAccessMode::kGet)),
     m_historizers({m_loadEnergy, m_resetLoadEnergy, m_loadPower, m_dimmerMode, m_dimmer, m_overCurrent})
{
}

const std::string& CProfile_D2_01_09::profile() const
{
   static const std::string Profile("D2-01-09");
   return Profile;
}

const std::string& CProfile_D2_01_09::title() const
{
   static const std::string Title(R"(Electronic dimmer with energy measurement and local control)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_09::allHistorizers() const
{
   return m_historizers;
}

void CProfile_D2_01_09::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler)
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
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_09::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler)
{
   // This device supports several RORG messages
   // We just use the VLD telegram
   if (rorg != CRorgs::ERorgIds::kVLD_Telegram)
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();

   switch (static_cast<CProfile_D2_01_Common::E_D2_01_Command>(bitset_extract(data, 4, 4)))  // NOLINT(clang-diagnostic-switch-enum)
   {
   case CProfile_D2_01_Common::E_D2_01_Command::kActuatorStatusResponse:
      {
         return CProfile_D2_01_Common::extractActuatorStatusResponse(rorg,
                                                                     data,
                                                                     CProfile_D2_01_Common::NoChannel1,
                                                                     m_dimmer,
                                                                     CProfile_D2_01_Common::NoPowerFailure,
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
   default:
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }
}

void CProfile_D2_01_09::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) {
   if (keyword == m_dimmer->getKeyword())
   {
      m_dimmer->setCommand(commandBody);

      CProfile_D2_01_Common::sendActuatorSetOutputCommandDimming(messageHandler,
                                                                 senderId,
                                                                 m_deviceId,
                                                                 CProfile_D2_01_Common::EOutputChannel::kAllOutputChannels,
                                                                 m_dimmerMode->get(),
                                                                 m_dimmer->get());
   }
   else if (keyword == m_dimmerMode->getKeyword())
   {
      m_dimmerMode->setCommand(commandBody);
      // Nothing to do more, this keyword is at internal-usage only.
      // It will be used at next dimmer value change.
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

void CProfile_D2_01_09::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler)
{
   const auto taughtInAllDevices = deviceConfiguration.get<std::string>("taughtIn") == "allDevices";
   const auto defaultState = deviceConfiguration.get<CProfile_D2_01_Common::EDefaultState>("defaultState");
   const auto dimTimer1 = deviceConfiguration.get<double>("dimTimer1");
   const auto dimTimer2 = deviceConfiguration.get<double>("dimTimer2");
   const auto dimTimer3 = deviceConfiguration.get<double>("dimTimer3");

   // CMD 0x2 - Actuator Set Local
   CProfile_D2_01_Common::sendActuatorSetLocalCommand(messageHandler,
                                                      senderId,
                                                      m_deviceId,
                                                      CProfile_D2_01_Common::EOutputChannel::kAllOutputChannels,
                                                      false,
                                                      taughtInAllDevices,
                                                      false,
                                                      false,
                                                      defaultState,
                                                      dimTimer1,
                                                      dimTimer2,
                                                      dimTimer3);


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
