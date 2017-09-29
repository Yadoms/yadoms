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
     m_inputEnergy(boost::make_shared<yApi::historization::CEnergy>("Input energy")),
     m_inputPower(boost::make_shared<yApi::historization::CPower>("Input power")),
     m_loadEnergy(boost::make_shared<yApi::historization::CEnergy>("Load energy")),
     m_loadPower(boost::make_shared<yApi::historization::CPower>("Load power")),
     m_historizers({m_channel, m_inputEnergy, m_inputPower,m_loadEnergy,m_loadPower})
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
         return historizers;
      }
   case CProfile_D2_01_Common::kActuatorMeasurementResponse:
      {
         // Return only the concerned historizer
         std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

         auto ioChannel = bitset_extract(data, 11, 5);
         auto unit = static_cast<CProfile_D2_01_Common::E_D2_01_MeasurementUnit>(bitset_extract(data, 8, 3));
         auto rawValue = bitset_extract(data, 16, 32);

         Poco::Int64 energyValueWh = 0;
         auto powerValueW = 0.0;
         switch (unit)
         {
         case CProfile_D2_01_Common::kEnergyWs:
            energyValueWh = static_cast<Poco::Int64>(rawValue) * 3600;
            break;
         case CProfile_D2_01_Common::kEnergyWh:
            energyValueWh = static_cast<Poco::Int64>(rawValue);
            break;
         case CProfile_D2_01_Common::kEnergyKWh:
            energyValueWh = static_cast<Poco::Int64>(rawValue) * 1000;
            break;
         case CProfile_D2_01_Common::kPowerW:
            powerValueW = static_cast<double>(rawValue);
            break;
         case CProfile_D2_01_Common::kPowerKW:
            powerValueW = static_cast<double>(rawValue) * 1000;
            break;
         default:
            YADOMS_LOG(information) << "Profile " << profile() << " : received unsupported unit value" << unit;
            break;
         }

         switch (ioChannel)
         {
         case 0: // Output channel
            {
               switch (unit)
               {
               case CProfile_D2_01_Common::kEnergyWs:
               case CProfile_D2_01_Common::kEnergyWh:
               case CProfile_D2_01_Common::kEnergyKWh:
                  m_loadEnergy->set(energyValueWh);
                  historizers.push_back(m_loadEnergy);
                  break;
               case CProfile_D2_01_Common::kPowerW:
               case CProfile_D2_01_Common::kPowerKW:
                  m_loadPower->set(powerValueW);
                  historizers.push_back(m_loadPower);
                  break;
               default:
                  YADOMS_LOG(information) << "Profile " << profile() << " : received unsupported unit value for output channel" << unit;
                  break;
               }
               break;
            }
         case 0x1F: // Input channel
            switch (unit)
            {
            case CProfile_D2_01_Common::kEnergyWs:
            case CProfile_D2_01_Common::kEnergyWh:
            case CProfile_D2_01_Common::kEnergyKWh:
               m_inputEnergy->set(energyValueWh);
               historizers.push_back(m_inputEnergy);
               break;
            case CProfile_D2_01_Common::kPowerW:
            case CProfile_D2_01_Common::kPowerKW:
               m_inputPower->set(powerValueW);
               historizers.push_back(m_inputPower);
               break;
            default:
               YADOMS_LOG(information) << "Profile " << profile() << " : received unsupported unit value for input channel" << unit;
               break;
            }
            break;
         default:
            YADOMS_LOG(information) << "Profile " << profile() << " : received unsupported ioChannel value " << ioChannel;
            break;
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
   CProfile_D2_01_Common::sendActuatorSetOutputCommandSwitching(messageHandler,
                                                                senderId,
                                                                m_deviceId,
                                                                CProfile_D2_01_Common::kOutputChannel1,
                                                                commandBody == "1");
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

   // Configure for both power and energy measure
   CProfile_D2_01_Common::sendActuatorSetMeasurementCommand(messageHandler,
                                                            senderId,
                                                            m_deviceId,
                                                            CProfile_D2_01_Common::kOutputChannel1,
                                                            false,
                                                            minEnergyMeasureRefreshTime,
                                                            maxEnergyMeasureRefreshTime);
   CProfile_D2_01_Common::sendActuatorSetMeasurementCommand(messageHandler, //TODO revoir mesure
                                                            senderId,
                                                            m_deviceId,
                                                            CProfile_D2_01_Common::kOutputChannel1,
                                                            true,
                                                            minEnergyMeasureRefreshTime,
                                                            maxEnergyMeasureRefreshTime);
}
