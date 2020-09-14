#include "stdafx.h"
#include "Profile_A5_11_02.h"
#include "../bitsetHelpers.hpp"

CProfile_A5_11_02::CProfile_A5_11_02(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_controllerValue(boost::make_shared<yApi::historization::CDimmable>("ControllerValue", yApi::EKeywordAccessMode::kGet)),
     m_automaticFan(boost::make_shared<specificHistorizers::CFan4Speeds>("FanAutomaticSpeed")),
     m_manualFan(boost::make_shared<specificHistorizers::CFan4Speeds>("FanManualSpeed")),
     m_setPoint(boost::make_shared<yApi::historization::CTemperature>("SetPoint")),
     m_alarm(boost::make_shared<yApi::historization::CSwitch>("Alarm", yApi::EKeywordAccessMode::kGet)),
     m_controllerModeHeating(boost::make_shared<yApi::historization::CSwitch>("ControllerModeHeating", yApi::EKeywordAccessMode::kGet)),
     m_controllerModeHCooling(boost::make_shared<yApi::historization::CSwitch>("ControllerModeHCooling", yApi::EKeywordAccessMode::kGet)),
     m_controllerStateOverriden(boost::make_shared<yApi::historization::CSwitch>("ControllerStateOverriden", yApi::EKeywordAccessMode::kGet)),
     m_energyHoldOff(boost::make_shared<yApi::historization::CSwitch>("EnergyHoldOff", yApi::EKeywordAccessMode::kGet)),
     m_occupancy(boost::make_shared<yApi::historization::CSwitch>("Occupancy", yApi::EKeywordAccessMode::kGet)),
     m_historizers({
        m_controllerValue, m_automaticFan, m_manualFan, m_setPoint, m_alarm, m_controllerModeHeating,
        m_controllerModeHCooling, m_controllerStateOverriden, m_energyHoldOff, m_occupancy
     })
{
}

const std::string& CProfile_A5_11_02::profile() const
{
   static const std::string Profile("A5-11-02");
   return Profile;
}

const std::string& CProfile_A5_11_02::title() const
{
   static const std::string Title(
      "Controller Status - Temperature Controller Output");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_11_02::allHistorizers() const
{
   return m_historizers;
}

void CProfile_A5_11_02::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Read-only device
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_11_02::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   m_controllerValue->set(static_cast<int>(bitset_extract(data, 0, 8)) * 100 / 255);
   historizers.push_back(m_controllerValue);

   switch (bitset_extract(data, 8, 8))
   {
   case 0:
      m_manualFan->set(specificHistorizers::EFan4Speeds::kSpeed0);
      historizers.push_back(m_manualFan);
      break;
   case 1:
      m_manualFan->set(specificHistorizers::EFan4Speeds::kSpeed1);
      historizers.push_back(m_manualFan);
      break;
   case 2:
      m_manualFan->set(specificHistorizers::EFan4Speeds::kSpeed2);
      historizers.push_back(m_manualFan);
      break;
   case 3:
      m_manualFan->set(specificHistorizers::EFan4Speeds::kSpeed3);
      historizers.push_back(m_manualFan);
      break;
   case 16:
      m_automaticFan->set(specificHistorizers::EFan4Speeds::kSpeed0);
      historizers.push_back(m_automaticFan);
      break;
   case 17:
      m_automaticFan->set(specificHistorizers::EFan4Speeds::kSpeed1);
      historizers.push_back(m_automaticFan);
      break;
   case 18:
      m_automaticFan->set(specificHistorizers::EFan4Speeds::kSpeed2);
      historizers.push_back(m_automaticFan);
      break;
   case 19:
      m_automaticFan->set(specificHistorizers::EFan4Speeds::kSpeed3);
      historizers.push_back(m_automaticFan);
      break;
   case 255:
      break;
   default:
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : Fan stage = " << bitset_extract(status, 8, 8);
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   m_setPoint->set(bitset_extract(status, 16, 8) * 51.2 / 255);
   historizers.push_back(m_setPoint);

   m_alarm->set(bitset_extract(status, 24, 1) ? true : false);
   historizers.push_back(m_alarm);

   switch (bitset_extract(data, 25, 2))
   {
   case 1:
      m_controllerModeHeating->set(true);
      historizers.push_back(m_controllerModeHeating);
      break;
   case 2:
      m_controllerModeHCooling->set(true);
      historizers.push_back(m_controllerModeHCooling);
      break;
   case 3:
      m_controllerModeHeating->set(false);
      m_controllerModeHCooling->set(false);
      historizers.push_back(m_controllerModeHeating);
      historizers.push_back(m_controllerModeHCooling);
      break;
   default:
      YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
         " : Controller mode = " << bitset_extract(status, 25, 2);
      return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
   }

   m_controllerStateOverriden->set(bitset_extract(status, 27, 1) ? true : false);
   historizers.push_back(m_controllerStateOverriden);

   m_energyHoldOff->set(bitset_extract(status, 29, 1) ? true : false);
   historizers.push_back(m_energyHoldOff);

   switch (bitset_extract(data, 30, 2))
   {
   case 0:
      m_occupancy->set(true);
      historizers.push_back(m_occupancy);
      break;
   case 1:
      m_occupancy->set(false);
      historizers.push_back(m_occupancy);
      break;
   default:
      break;
   }

   return historizers;
}

void CProfile_A5_11_02::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_A5_11_02::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Device supports no configuration
}
