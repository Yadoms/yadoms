#include "stdafx.h"
#include "Profile_A5_11_04.h"
#include "../bitsetHelpers.hpp"

CProfile_A5_11_04::CProfile_A5_11_04(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_dimValue(boost::make_shared<yApi::historization::CDimmable>("DimValue", yApi::EKeywordAccessMode::kGet)),
     m_lampOperatingHours(boost::make_shared<yApi::historization::CDuration>("LampOperatingHours")),
     m_power(boost::make_shared<yApi::historization::CPower>("Power")),
     m_energy(boost::make_shared<yApi::historization::CEnergyDouble>("Energy")),
     m_current(boost::make_shared<yApi::historization::CCurrent>("Current")),
     m_voltage(boost::make_shared<yApi::historization::CVoltage>("Voltage")),
     m_color(boost::make_shared<yApi::historization::CColorRGB>("Color", yApi::EKeywordAccessMode::kGet)),
     m_errorState(boost::make_shared<yApi::historization::CSwitch>("ErrorState", yApi::EKeywordAccessMode::kGet)),
     m_errorMessage(boost::make_shared<yApi::historization::CText>("ErrorMessage")),
     m_serviceMode(boost::make_shared<yApi::historization::CSwitch>("ServiceModeActivated", yApi::EKeywordAccessMode::kGet)),
     m_lighting(boost::make_shared<yApi::historization::CSwitch>("Lighting", yApi::EKeywordAccessMode::kGet)),
     m_historizers({
        m_dimValue, m_lampOperatingHours, m_power, m_energy, m_current, m_voltage, m_color, m_errorState,
        m_errorMessage, m_serviceMode, m_lighting
     })
{
}

const std::string& CProfile_A5_11_04::profile() const
{
   static const std::string Profile("A5-11-04");
   return Profile;
}

const std::string& CProfile_A5_11_04::title() const
{
   static const std::string Title(
      "Controller Status - Extended Lighting Status");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_11_04::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_11_04::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_11_04::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   const auto parameter1 = bitset_extract(data, 0, 8);
   const auto parameter2 = bitset_extract(data, 8, 8);
   const auto parameter3 = bitset_extract(data, 16, 8);

   const auto mode = bitset_extract(data, 29, 2);
   switch (mode)
   {
   case 0:
      m_dimValue->set(static_cast<int>(parameter1) * 100 / 255);
      historizers.push_back(m_dimValue);
      if (bitset_extract(data, 25, 1))
      {
         m_lampOperatingHours->set(static_cast<double>(parameter2 << 8 || parameter3) * 3600.0);
         historizers.push_back(m_lampOperatingHours);
      }
      break;
   case 1:
      m_color->setRGB(parameter1, parameter2, parameter3);
      historizers.push_back(m_color);
      break;
   case 2:
      {
         const auto energyMeteringValue = parameter1 << 8 || parameter2;
         switch (parameter3)
         {
         case 0: // mW
            m_power->set(static_cast<double>(energyMeteringValue) / 1000.0);
            historizers.push_back(m_power);
            break;
         case 1: // W
            m_power->set(static_cast<double>(energyMeteringValue));
            historizers.push_back(m_power);
            break;
         case 2: // kW
            m_power->set(static_cast<double>(energyMeteringValue) * 1000.0);
            historizers.push_back(m_power);
            break;
         case 3: // MW
            m_power->set(static_cast<double>(energyMeteringValue) * 1000000.0);
            historizers.push_back(m_power);
            break;
         case 4: // Wh
            m_energy->set(static_cast<double>(energyMeteringValue));
            historizers.push_back(m_energy);
            break;
         case 5: // kWh
            m_energy->set(static_cast<double>(energyMeteringValue) * 1000.0);
            historizers.push_back(m_energy);
            break;
         case 6: // MWh
            m_energy->set(static_cast<double>(energyMeteringValue) * 1000000.0);
            historizers.push_back(m_energy);
            break;
         case 7: // GWh
            m_energy->set(static_cast<double>(energyMeteringValue) * 1000000000.0);
            historizers.push_back(m_energy);
            break;
         case 8: // mA
            m_current->set(static_cast<double>(energyMeteringValue) / 1000.0);
            historizers.push_back(m_current);
            break;
         case 9: // 1/10 A
            m_current->set(static_cast<double>(energyMeteringValue) / 10.0);
            historizers.push_back(m_current);
            break;
         case 10: // mV
            m_voltage->set(static_cast<double>(energyMeteringValue) / 1000.0);
            historizers.push_back(m_voltage);
            break;
         case 11: // 1/10 V
            m_voltage->set(static_cast<double>(energyMeteringValue) / 10.0);
            historizers.push_back(m_voltage);
            break;
         default:
            break;
         }
         break;
      }
   default:
      break;
   }

   switch (bitset_extract(data, 26, 2))
   {
   case 0:
      m_errorState->set(false);
      m_errorMessage->set(std::string());
      historizers.push_back(m_errorState);
      historizers.push_back(m_errorMessage);
      break;
   case 1:
      m_errorState->set(true);
      m_errorMessage->set("Lamp failure");
      historizers.push_back(m_errorState);
      historizers.push_back(m_errorMessage);
      break;
   case 2:
      m_errorState->set(true);
      m_errorMessage->set("Internal Failure");
      historizers.push_back(m_errorState);
      historizers.push_back(m_errorMessage);
      break;
   case 3:
      m_errorState->set(true);
      m_errorMessage->set("Failure on external periphery");
      historizers.push_back(m_errorState);
      historizers.push_back(m_errorMessage);
      break;
   default:
      break;
   }

   m_serviceMode->set(bitset_extract(status, 24, 1) ? true : false);
   historizers.push_back(m_serviceMode);

   m_lighting->set(bitset_extract(data, 31, 1) ? true : false);
   historizers.push_back(m_lighting);

   return historizers;
}

void CProfile_A5_11_04::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_11_04::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
