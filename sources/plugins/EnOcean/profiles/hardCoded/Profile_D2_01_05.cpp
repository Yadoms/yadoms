#include "stdafx.h"
#include "Profile_D2_01_05.h"
#include "../bitsetHelpers.hpp"
#include "../../message/RadioErp1SendMessage.h"
#include "Profile_D2_01_Common.h"
#include <shared/Log.h>

CProfile_D2_01_05::CProfile_D2_01_05(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_inputEnergy(boost::make_shared<yApi::historization::CEnergy>("Input energy")),
     m_inputPower(boost::make_shared<yApi::historization::CPower>("Input power")),
     m_loadEnergy(boost::make_shared<yApi::historization::CEnergy>("Load energy")),
     m_loadPower(boost::make_shared<yApi::historization::CPower>("Load power")),
     m_dimmerMode(boost::make_shared<specificHistorizers::CDimmerModeHistorizer>("DimmerMode")),
     m_dimmer(boost::make_shared<yApi::historization::CDimmable>("Dimmer", yApi::EKeywordAccessMode::kGetSet)),
     m_historizers({m_inputEnergy, m_inputPower, m_loadEnergy, m_loadPower, m_dimmer , m_dimmerMode})
{
}

CProfile_D2_01_05::~CProfile_D2_01_05()
{
}

const std::string& CProfile_D2_01_05::profile() const
{
   static const std::string profile("D2-01-05");
   return profile;
}

const std::string& CProfile_D2_01_05::title() const
{
   static const std::string title("Electronic dimmer with energy measurement and local control");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_05::allHistorizers() const
{
   return m_historizers;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_05::states(unsigned char rorg,
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

                  // Power is configured to be received automaticaly.
                  // As we can not receive both data (power + energy) automaticaly,
                  // we ask for Energy just after receiving Power.
                  CProfile_D2_01_Common::sendActuatorMeasurementQuery(messageHandler,
                                                                      senderId,
                                                                      m_deviceId,
                                                                      CProfile_D2_01_Common::kOutputChannel1); //TODO voir s'il faut le faire dans les autres profils
                  break;
               default:
                  YADOMS_LOG(information) << "Profile " << profile() << " : received unsupported unit value for output channel" << unit;
                  break;
               }
               break;
            }
         case 0x1F: // Input channel //TODO utile ?
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

void CProfile_D2_01_05::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   if (keyword == m_dimmer->getKeyword())
   {
   }
   else if (keyword == m_dimmer->getKeyword())
   {
      // Nothing to do, this keyword is at internal-usage only.
      // It will be used at next dimmer value change.
      return;
   }
   else
   {
      std::ostringstream oss;
      oss << "Device " << m_deviceId << " (" << profile() << ") : send command on unsupported keyword " << keyword;
      YADOMS_LOG(information) << oss.str();
      throw std::logic_error(oss.str());
   }

   CProfile_D2_01_Common::sendActuatorSetOutputCommandDimming(messageHandler,
                                                              senderId,
                                                              m_deviceId,
                                                              CProfile_D2_01_Common::kOutputChannel1,
                                                              m_dimmerMode->get(),
                                                              std::stoul(commandBody));//TODO utiliser m_dimmer->get() ? (et vérifier tous les appels à CProfile_D2_01_Common::sendActuatorSetOutputCommandDimming
}

void CProfile_D2_01_05::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   auto localControl = deviceConfiguration.get<std::string>("localControl") == "enable";
   auto taughtInAllDevices = deviceConfiguration.get<std::string>("taughtIn") == "allDevices";
   auto userInterfaceDayMode = deviceConfiguration.get<std::string>("userInterfaceMode") == "dayMode";
   auto defaultState = deviceConfiguration.get<CProfile_D2_01_Common::EDefaultState>("defaultState");
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
                                                      userInterfaceDayMode,
                                                      false,
                                                      defaultState,
                                                      dimTimer1,
                                                      dimTimer2,
                                                      dimTimer3);


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
